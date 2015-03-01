#include "crcgenerator.h"

namespace NsCRC
{
    QBitArray generate(QBitArray aPayload, QBitArray aPoly)
    {
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

    QBitArray & 	operator<< ( QBitArray & out, int ba )
    {
        //qDebug()<<ba<<" before"<<out;


        if( 0 == out.size() || 0 == ba )
        {
            //do nothing
        }
        else if( 1 == out.size() )
        {
            out[0] = false;
        }
        else
        {
            for(int k=0; k<ba; k++)
            {
                for(int i=out.size()-1; i>0; i--)
                {
                    out[i] = out[i-1];
                    //qDebug()<<ba<<" shifting"<<out;
                }
                out[0] = false;
            }
        }


        //qDebug()<<ba<<" after"<<out;
        return out;
    }

    QBitArray & 	operator>> ( QBitArray & in, int ba )
    {
        if( 0 == in.size() || 0 == ba )
        {
            //do nothing
        }
        else if( 1 == in.size() )
        {
            in[0] = false;
        }
        else
        {
            for(int k=0; k<ba; k++)
            {
                for(int i=0; i<in.size()-1; i++)
                {
                    in[i] = in[i+1];
                }
                in[in.size()-1] = false;
            }
        }

        return in;
    }
}
