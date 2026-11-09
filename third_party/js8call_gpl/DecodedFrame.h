// -*- Mode: C++ -*-
/*
 * JS8 frame unpacking, taken from DecodedText of JS8Call 2.2.0
 * (VK3ACF 2013, Jordan Sherer KN4CRD 2018), GPL-3.0-or-later.
 *
 * MultiModeModem keeps only the JS8 constructor (12-character frame plus
 * the 3 transmission-type bits) and the tryUnpack* chain; the WSJT-X
 * column parser and the Fortran stdmsg check are not needed.
 */
#ifndef JS8_DECODEDFRAME_H
#define JS8_DECODEDFRAME_H

#include <QString>
#include <QStringList>

namespace js8 {

class DecodedFrame
{
public:
    DecodedFrame(QString const &frame, int bits, int submode);

    quint8 frameType() const { return frameType_; }
    QString frame() const { return frame_; }
    QString extra() const { return extra_; }
    QString compoundCall() const { return compound_; }
    bool isCompound() const { return !compound_.isEmpty(); }
    bool isHeartbeat() const { return isHeartbeat_; }
    bool isAlt() const { return isAlt_; }
    QStringList directedMessage() const { return directed_; }
    bool isDirectedMessage() const { return !directed_.isEmpty() && directed_.length() > 2; }
    QString message() const { return message_; }
    int bits() const { return bits_; }
    int submode() const { return submode_; }
    bool isStandardMessage() const { return is_standard_; }

private:
    bool tryUnpack();
    bool tryUnpackHeartbeat();
    bool tryUnpackCompound();
    bool tryUnpackDirected();
    bool tryUnpackData();
    bool tryUnpackFastData();

    quint8 frameType_;
    bool isHeartbeat_ = false;
    bool isAlt_ = false;
    QString compound_;
    QString extra_;
    QStringList directed_;
    QString message_;
    bool is_standard_ = false;
    int bits_ = 0;
    int submode_ = 0;
    QString frame_;
};

} // namespace js8

#endif // JS8_DECODEDFRAME_H
