#ifndef JS8_JSC_H
#define JS8_JSC_H

/**
 * (C) 2018 Jordan Sherer <kn4crd@gmail.com> - All Rights Reserved
 *
 * MultiModeModem: the 262144-word tables are read once from the resource
 * :/js8/jsc_tables.txt (generated from JS8Call 2.2.0 by
 * tools/js8_generate_jsc_tables.py) instead of 12 MB of C++ initialisers.
 **/

#include <QList>
#include <QPair>
#include <QString>
#include <QVector>

namespace js8 {

typedef QPair<QVector<bool>, quint32> CodewordPair;        // Tuple(Codeword, N) where N = number of characters
typedef QVector<bool> Codeword;                            // Codeword bit vector

typedef struct Tuple{
    char const * str;
    int size;
    int index;
} Tuple;

class JSC
{
public:
    static Codeword codeword(quint32 index, bool separate, quint32 bytesize, quint32 s, quint32 c);
    static QList<CodewordPair> compress(QString text);
    static QString decompress(Codeword const& bits);

    static bool exists(QString w, quint32 *pIndex);
    static quint32 lookup(QString w, bool *ok);
    static quint32 lookup(char const* b, bool *ok);

    static const quint32 size = 262144;
    static const quint32 prefixSize = 103;

    static const Tuple *map();
    static const Tuple *list();
    static const Tuple *prefix();
    /// False when the table resource is missing or damaged.
    static bool tablesLoaded();
};

} // namespace js8

#endif // JS8_JSC_H
