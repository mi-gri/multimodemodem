#include "modems/rtty/RttyDecoder.h"
#include "audio/AudioBlock.h"

#include <QCoreApplication>
#include <QJsonDocument>
#include <QJsonObject>
#include <QString>
#include <QVector>

#include <algorithm>
#include <cmath>
#include <iostream>
#include <numeric>
#include <random>

namespace {
constexpr double kPi = 3.14159265358979323846;

int editDistance(const QString &a, const QString &b)
{
    QVector<int> row(b.size() + 1);
    std::iota(row.begin(), row.end(), 0);
    for (int i = 0; i < a.size(); ++i) {
        int previous = row[0];
        row[0] = i + 1;
        for (int j = 0; j < b.size(); ++j) {
            const int old = row[j + 1];
            row[j + 1] = std::min({row[j + 1] + 1,
                                   row[j] + 1,
                                   previous + (a[i] != b[j])});
            previous = old;
        }
    }
    return row.last();
}

double cer(const QString &expected, const QString &decoded)
{
    const auto denominator = expected.isEmpty() ? 1 : expected.size();
    return static_cast<double>(editDistance(expected, decoded)) /
           static_cast<double>(denominator);
}

QVector<float> reference(const QString &text,
                         int rate,
                         double baud = 45.45,
                         double frequencyOffset = 0.0,
                         double snrDb = 100.0,
                         double markScale = 1.0,
                         double spaceScale = 1.0)
{
    const QString letters = QString::fromLatin1("\0E\nA SIU\rDRJNFCKTZLWHYPQOBG\0MXV\0", 32);
    const QString figures = QString::fromLatin1("\0003\n- '\0707\r$4\a,!:(5\")2#6019?&\0./;\0", 32);
    QVector<float> out;
    double bitPosition = 0.0;
    double phase = 0.0;
    std::mt19937 rng(0x52545459u);
    std::normal_distribution<double> gaussian;

    auto appendBit = [&](bool mark, double bitLength) {
        bitPosition += bitLength;
        const int end = std::lround(bitPosition * rate / baud);
        const double frequency = (mark ? 2125.0 : 2295.0) + frequencyOffset;
        const double scale = mark ? markScale : spaceScale;
        const double phaseIncrement = 2.0 * kPi * frequency / rate;
        while (out.size() < end) {
            double sample = 0.20 * scale * std::sin(phase);
            if (snrDb < 90.0) {
                sample += (0.20 / std::sqrt(2.0)) * std::pow(10.0, -snrDb / 20.0) * gaussian(rng);
            }
            out.append(static_cast<float>(sample));
            phase = std::fmod(phase + phaseIncrement, 2.0 * kPi);
        }
    };

    auto appendCode = [&](int code) {
        appendBit(false, 1.0);
        for (int bit = 0; bit < 5; ++bit) {
            appendBit((code & (1 << bit)) != 0, 1.0);
        }
        appendBit(true, 1.5);
    };

    appendBit(true, 20.0);
    appendCode(31);
    bool figuresMode = false;
    for (QChar ch : text.toUpper()) {
        int code = letters.indexOf(ch);
        bool needsFigures = false;
        if (code < 0) {
            code = figures.indexOf(ch);
            needsFigures = true;
        }
        if (code < 0) {
            continue;
        }
        if (needsFigures != figuresMode) {
            appendCode(needsFigures ? 27 : 31);
            figuresMode = needsFigures;
        }
        appendCode(code);
    }
    appendBit(true, 20.0);
    return out;
}

// Independent slow fading of Mark and Space (selective fading on wide
// shifts): each gain is the magnitude of a sum of random-phase sinusoids
// with Doppler up to fadeHz, normalised to unit mean power.
QVector<float> fadedReference(const QString &text,
                              int rate,
                              double baud,
                              double markHz,
                              double shiftHz,
                              double snrDb,
                              double fadeHz,
                              unsigned seed)
{
    const QString letters = QString::fromLatin1("\0E\nA SIU\rDRJNFCKTZLWHYPQOBG\0MXV\0", 32);
    const QString figures = QString::fromLatin1("\0003\n- '\0707\r$4\a,!:(5\")2#6019?&\0./;\0", 32);
    std::mt19937 rng(seed);
    std::uniform_real_distribution<double> uniform(0.0, 1.0);
    std::normal_distribution<double> gaussian;
    struct Path { double frequency[8]; double phase[8]; };
    auto makePath = [&]() {
        Path path{};
        for (int k = 0; k < 8; ++k) {
            path.frequency[k] = fadeHz * std::cos(2.0 * kPi * uniform(rng));
            path.phase[k] = 2.0 * kPi * uniform(rng);
        }
        return path;
    };
    const Path markPath = makePath();
    const Path spacePath = makePath();
    auto gain = [&](const Path &path, double t) {
        if (fadeHz <= 0.0) {
            return 1.0;
        }
        double re = 0.0, im = 0.0;
        for (int k = 0; k < 8; ++k) {
            const double angle = 2.0 * kPi * path.frequency[k] * t + path.phase[k];
            re += std::cos(angle);
            im += std::sin(angle);
        }
        return std::sqrt((re * re + im * im) / 8.0);
    };
    QVector<float> out;
    double bitPosition = 0.0;
    double phase = 0.0;
    auto appendBit = [&](bool mark, double bitLength) {
        bitPosition += bitLength;
        const int end = std::lround(bitPosition * rate / baud);
        const double increment = 2.0 * kPi * (mark ? markHz : markHz + shiftHz) / rate;
        while (out.size() < end) {
            const double t = static_cast<double>(out.size()) / rate;
            double sample = 0.20 * gain(mark ? markPath : spacePath, t) * std::sin(phase);
            sample += (0.20 / std::sqrt(2.0)) * std::pow(10.0, -snrDb / 20.0) * gaussian(rng);
            out.append(static_cast<float>(sample));
            phase = std::fmod(phase + increment, 2.0 * kPi);
        }
    };
    auto appendCode = [&](int code) {
        appendBit(false, 1.0);
        for (int bit = 0; bit < 5; ++bit) {
            appendBit((code & (1 << bit)) != 0, 1.0);
        }
        appendBit(true, 1.5);
    };
    appendBit(true, 20.0);
    appendCode(31);
    bool figuresMode = false;
    for (QChar ch : text.toUpper()) {
        int code = letters.indexOf(ch);
        bool needsFigures = false;
        if (code < 0) {
            code = figures.indexOf(ch);
            needsFigures = true;
        }
        if (code < 0) {
            continue;
        }
        if (needsFigures != figuresMode) {
            appendCode(needsFigures ? 27 : 31);
            figuresMode = needsFigures;
        }
        appendCode(code);
    }
    appendBit(true, 20.0);
    return out;
}

QString decodeTones(const QVector<float> &samples, int rate, double baud,
                    double markHz, double shiftHz, bool narrow)
{
    RttyDecoder decoder;
    decoder.setVisualizationEnabled(false);
    decoder.setBaudRate(baud);
    decoder.setTones(markHz, markHz + shiftHz);
    decoder.setNarrowFilterEnabled(narrow);
    QString text;
    QObject::connect(&decoder, &RttyDecoder::characterReceived,
                     [&](const QString &piece) { text += piece; });
    for (int pos = 0; pos < samples.size(); pos += 4800) {
        AudioBlock block;
        block.sampleRate = rate;
        block.firstSampleIndex = pos;
        block.samples = samples.mid(pos, 4800);
        decoder.processAudioBlock(block);
    }
    return text;
}

QVector<float> addCw(const QVector<float> &input,
                     int rate,
                     double frequencyHz,
                     double relativeDb)
{
    double squareSum = 0.0;
    for (float sample : input) {
        squareSum += static_cast<double>(sample) * sample;
    }
    const auto sampleCount = input.isEmpty() ? 1 : input.size();
    const double signalRms = std::sqrt(squareSum / static_cast<double>(sampleCount));
    const double amplitude = signalRms * std::pow(10.0, relativeDb / 20.0) * std::sqrt(2.0);

    QVector<float> output(input.size());
    double phase = 0.31;
    const double increment = 2.0 * kPi * frequencyHz / rate;
    double peak = 0.0;
    for (int i = 0; i < input.size(); ++i) {
        const double mixed = static_cast<double>(input[i]) + amplitude * std::sin(phase);
        output[i] = static_cast<float>(mixed);
        peak = std::max(peak, std::abs(mixed));
        phase = std::fmod(phase + increment, 2.0 * kPi);
    }
    if (peak > 0.95) {
        const double gain = 0.95 / peak;
        for (float &sample : output) {
            sample = static_cast<float>(sample * gain);
        }
    }
    return output;
}

QVector<float> noiseOnly(int samples, double rms)
{
    std::mt19937 rng(0x4e4f4953u);
    std::normal_distribution<double> gaussian(0.0, rms);
    QVector<float> out(samples);
    for (float &sample : out) {
        sample = static_cast<float>(gaussian(rng));
    }
    return out;
}

QString decode(const QVector<float> &samples,
               int rate,
               bool narrow = false,
               double decoderOffsetHz = 0.0,
               int chunk = 1024)
{
    RttyDecoder decoder;
    decoder.setVisualizationEnabled(false);
    decoder.setNarrowFilterEnabled(narrow);
    if (decoderOffsetHz != 0.0) {
        decoder.retuneTones(2125.0 + decoderOffsetHz, 2295.0 + decoderOffsetHz);
    }
    QString text;
    QObject::connect(&decoder, &RttyDecoder::characterReceived,
                     [&](const QString &piece) { text += piece; });
    for (int pos = 0; pos < samples.size(); pos += chunk) {
        AudioBlock block;
        block.sampleRate = rate;
        block.firstSampleIndex = pos;
        block.samples = samples.mid(pos, chunk);
        decoder.processAudioBlock(block);
    }
    return text;
}

void report(const QString &name,
            const QString &expected,
            const QString &decoded,
            bool narrow)
{
    QJsonObject object;
    object.insert(QStringLiteral("scenario"), name);
    object.insert(QStringLiteral("narrow"), narrow);
    object.insert(QStringLiteral("expected_chars"), expected.size());
    object.insert(QStringLiteral("decoded_chars"), decoded.size());
    object.insert(QStringLiteral("edit_distance"), editDistance(expected, decoded));
    object.insert(QStringLiteral("cer"), cer(expected, decoded));
    std::cout << QJsonDocument(object).toJson(QJsonDocument::Compact).constData() << '\n';
}
}

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv);
    bool ok = true;
    auto require = [&](bool condition, const char *name) {
        std::cout << (condition ? "PASS " : "FAIL ") << name << '\n';
        ok &= condition;
    };

    const int rate = 48000;
    const QString message = QStringLiteral("CQ CQ TEST DE IZ6NNH IZ6NNH 599 15 TU ").repeated(3);

    auto clean = reference(message, rate);
    auto out = decode(clean, rate);
    report(QStringLiteral("clean"), message, out, false);
    require(out == message, "clean 45.45/170 decode");

    auto awgnMinus6 = reference(message, rate, 45.45, 0.0, -6.0);
    out = decode(awgnMinus6, rate);
    report(QStringLiteral("awgn_-6_db"), message, out, false);
    require(cer(message, out) <= 0.10, "AWGN -6 dB CER <= 10%");

    const double fade18 = std::pow(10.0, -18.0 / 20.0);
    auto markFade = reference(message, rate, 45.45, 0.0, 100.0, fade18, 1.0);
    out = decode(markFade, rate);
    report(QStringLiteral("mark_fade_-18_db"), message, out, false);
    require(cer(message, out) <= 0.02, "Mark selective fade -18 dB");

    auto spaceFade = reference(message, rate, 45.45, 0.0, 100.0, 1.0, fade18);
    out = decode(spaceFade, rate);
    report(QStringLiteral("space_fade_-18_db"), message, out, false);
    require(cer(message, out) <= 0.02, "Space selective fade -18 dB");

    auto strongCw = addCw(clean, rate, 2210.0 + 300.0, 20.0);
    out = decode(strongCw, rate);
    report(QStringLiteral("cw_plus20_db_offset300"), message, out, false);
    require(cer(message, out) <= 0.03, "CW +20 dB at +300 Hz");

    auto veryStrongCw = addCw(clean, rate, 2210.0 + 400.0, 30.0);
    out = decode(veryStrongCw, rate);
    report(QStringLiteral("cw_plus30_db_offset400"), message, out, false);
    require(cer(message, out) <= 0.08, "CW +30 dB at +400 Hz");

    auto closeCw = addCw(clean, rate, 2210.0 + 150.0, 10.0);
    const QString normalClose = decode(closeCw, rate, false);
    const QString narrowClose = decode(closeCw, rate, true);
    report(QStringLiteral("close_cw_normal"), message, normalClose, false);
    report(QStringLiteral("close_cw_narrow"), message, narrowClose, true);
    require(cer(message, narrowClose) <= 0.10, "narrow channel handles +10 dB CW at +150 Hz");
    require(cer(message, narrowClose) < cer(message, normalClose), "narrow channel improves close-CW case");

    auto mistuned = reference(message, rate, 45.45, 15.0);
    out = decode(mistuned, rate, false, 15.0);
    report(QStringLiteral("afc_retuned_plus15_hz"), message, out, false);
    require(cer(message, out) <= 0.02, "retuned channels follow +15 Hz offset");

    // Weather-service style 50 Bd / 450 Hz with independent Mark/Space
    // fading and noise, plus the amateur 45.45 Bd / 170 Hz at low SNR.
    const QString weather = QStringLiteral(
        "SA 14. 12Z: SW-W     5   6-7   2  M //\nSU 15. 00Z: W-NW   2-3        2.5 M //\n").repeated(3);
    double weatherCer = 0.0;
    for (unsigned seed = 1; seed <= 6; ++seed) {
        auto faded = fadedReference(weather, rate, 50.0, 2125.0, 450.0, -8.0, 1.5, seed);
        for (bool narrow : {false, true}) {
            out = decodeTones(faded, rate, 50.0, 2125.0, 450.0, narrow);
            report(QStringLiteral("dwd_50_450_fading_seed%1").arg(seed), weather, out, narrow);
            weatherCer += cer(weather, out) / 12.0;
        }
    }
    std::cout << "mean fading CER " << weatherCer << '\n';
    require(weatherCer <= 0.20, "50 Bd / 450 Hz selective fading mean CER <= 20% (was 23%)");
    double awgnCer = 0.0;
    for (unsigned seed = 1; seed <= 4; ++seed) {
        auto noisy = fadedReference(message, rate, 45.45, 2125.0, 170.0, -18.0, 0.0, seed);
        out = decodeTones(noisy, rate, 45.45, 2125.0, 170.0, false);
        report(QStringLiteral("awgn_-18_db_seed%1").arg(seed), message, out, false);
        awgnCer += cer(message, out) / 4.0;
    }
    std::cout << "mean awgn -18 dB CER " << awgnCer << '\n';
    require(awgnCer <= 0.50, "45.45 Bd / 170 Hz AWGN -18 dB mean CER <= 50% (was 64%)");

    const auto pureNoise = noiseOnly(clean.size(), 0.05);
    out = decode(pureNoise, rate);
    report(QStringLiteral("noise_only"), QString(), out, false);
    require(out.isEmpty(), "noise-only produces no characters");

    return ok ? 0 : 1;
}
