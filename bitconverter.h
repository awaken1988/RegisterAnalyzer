#ifndef BITCONVERTER_H
#define BITCONVERTER_H
#include <QtGlobal>
#include <QBitArray>
#include <QString>
#include <QDebug>
#include <algorithm>


namespace NsConverter
{
    QString toHexStr(const QBitArray& aContent);
    QString toBinStr(const QBitArray& aContent);

    bool fromHexStr(QBitArray& aContent, QString aValue, bool aResize=false);
    bool fromBinStr(QBitArray& aContent, QString aValue, bool aResize=false);

    QBitArray extract(const QBitArray& aContent, int aStartBit, int aEndBit);

    QString nippleToHex(quint8 aNipple);
    quint8 hexToNipple(char aNipple, bool& isValid);

}

#endif // BITCONVERTER_H
