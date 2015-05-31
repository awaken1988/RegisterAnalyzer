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

    //addons to QBitArray
    struct shift_t {
        shift_t(int aShifts)                : shifts(aShifts), resize(false)    {}
        shift_t(int aShifts, bool aResize)  : shifts(aShifts), resize(aResize)  {}
        int  shifts = 0;
        bool resize = false;
    };

    //TODO: returns << and >> really a ref???
    QBitArray & 	operator<< ( QBitArray& out, int ba );
    QBitArray & 	operator>> ( QBitArray& in, int  ba );
    QBitArray & 	operator<< ( QBitArray& out, shift_t ba );
    QBitArray & 	operator>> ( QBitArray& in, shift_t  ba );

    QBitArray mirror(const QBitArray& in);
    QBitArray invert(const QBitArray& in);

}

#endif // BITCONVERTER_H
