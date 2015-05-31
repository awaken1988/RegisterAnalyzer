#include "crcgenerator.h"
#include "bitconverter.h"

namespace NsCRC
{
    QBitArray generate(QBitArray aPayload, QBitArray aPoly)
    {
        using namespace NsConverter;

        const int n = aPoly.size();
        QBitArray shiftReg(n, false);

        for(int i=aPayload.size()-1; i>=0; i--)
        {
            if( shiftReg[n-1] != aPayload[i]  )
            {
                //qDebug()<<"--------------------";
                // qDebug()<<" poly "<<aPoly;
                //qDebug()<<" shiftxor before shift"<<shiftReg;
                shiftReg = shiftReg<<1;
                //qDebug()<<" shiftxor before xor"<<shiftReg;
                shiftReg ^= aPoly;
                // qDebug()<<" shiftxor after"<<shiftReg;
            }
            else
            {
                shiftReg = shiftReg<<1;
            }
        }

        return shiftReg;
    }

}
