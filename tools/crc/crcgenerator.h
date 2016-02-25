#ifndef CRCGENERATOR_H
#define CRCGENERATOR_H

#include <QBitArray>
#include "bitdef.h"
#include <QDebug>

namespace NsCRC
{
    QBitArray generate(QBitArray aPayload, QBitArray aPoly);

    template<int bitlen> QBitArray toQBitarray(quint64 aVal)
    {
        QBitArray ret(bitlen, false);

        //convert all bits to QBitArray
        for(int i=0; i<bitlen; i++)
        {
            if( (aVal>>i) & 0x1 )
                ret[i] = true;
        }

        return ret;
    }
    /*
    template<typename T> QBitArray toQBitarray(T aVal, bool aCutZero)
    {
        const int bitlen  = sizeof(aVal) * NsBitDef::BIT_PER_BYTE;
        QBitArray ret(bitlen, false);

        //convert all bits to QBitArray
        for(int i=0; i<bitlen; i++)
        {
            if( (aVal>>i) & 0x1 )
                ret[i] = true;
        }

        //cut
        if( aCutZero )
        {
            int cutPos=0;

            for(int i=0; i<bitlen; i++)
            {
                if( (aVal>>i) & 0x1 )
                    cutPos = i;
            }

            ret.truncate(cutPos+1);
        }

        return ret;
    }
    */
}



#endif // CRCGENERATOR_H
