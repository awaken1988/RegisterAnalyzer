#include "testbitconverter.h"


void TestBitConverter::convertFromHex()
{
    QBitArray bitVec(32, false);
    QString   importVal = "CAfEBeEF";

    NsConverter::fromHexStr(bitVec, importVal);

    QCOMPARE(NsConverter::toHexStr(bitVec), QString("CAFEBEEF"));
}


