#include "bitconverter.h"

//! Converting QBitArray to {bin,hex}-String (and back)
namespace NsConverter
{
    QString toHexStr(const QBitArray& aContent)
    {
        QString ret;
        const int contentSize = aContent.size();
        const int chunckSize = 4;

        for(int iBit=0; iBit<contentSize;)
        {
            const int currWidth = contentSize-iBit >= chunckSize ?
                                            chunckSize : contentSize-iBit;
            quint8 currNipple = 0x00;

            for(int kChunk=0; kChunk<currWidth; kChunk++)
            {
                currNipple |= aContent.at(iBit+kChunk) << kChunk;
            }

            ret.prepend( nippleToHex(currNipple) );

            iBit += currWidth;
        }

        return ret;
    }

    QString toBinStr(const QBitArray& aContent)
    {
        QString ret;

        for(int iBit=0; iBit<aContent.size(); iBit++)
        {
            ret.prepend( aContent.at(iBit) ? "1" : "0" );
        }

        return ret;
    }

    bool fromHexStr(QBitArray& aContent, QString aValue, bool aResize)
    {
        const int nippleWidth = 4;      //FIXME: substitue this with a global constant

         if(aResize)
            aContent.resize(aValue.size()*nippleWidth);

        aContent.fill(false);
        std::reverse(aValue.begin(), aValue.end());
        aValue = aValue.toUpper();

        for(int iNipple=0; iNipple<aValue.size(); iNipple++)
        {
            bool isCharValid=false;
            const char   currSymbol = aValue[iNipple].toLatin1();
            const quint8 currVal    = hexToNipple(currSymbol, isCharValid);

            if(!isCharValid)
                return false;


            for(int iBit=0; iBit<nippleWidth; iBit++)
            {
                const quint8    currBit         = (currVal>>iBit) & 0x1;
                const int       currBitIndex    = iNipple*nippleWidth + iBit;

                if( !currBit )
                    continue;

                if( currBitIndex >= aContent.size() )
                    return false;                       //error setting a value which is bigger than aContent

                 aContent.setBit(currBitIndex, currBit);
            }
        }

        return true;
    }

    bool fromBinStr(QBitArray& aContent, QString aValue, bool aResize)
    {
        if(aResize)
            aContent.resize(aValue.size());

        std::reverse(aValue.begin(), aValue.end());     //flip string

        for(int iBit=0; iBit<aValue.size(); iBit++)
        {
            if( aValue.size() > aContent.size() )
                return false;

            if( aValue.at(iBit) == QChar('0') )
                aContent.setBit(iBit, false);
            else if( aValue.at(iBit) == QChar('1') )
                aContent.setBit(iBit, true);
            else
                return false;
        }


        return true;
    }

    QBitArray extract(const QBitArray& aContent, int aStartBit, int aEndBit)
    {
        if( aContent.size() <= aEndBit )
            return QBitArray(0);
        if( aStartBit > aEndBit )
            return QBitArray(0);

        const int bitLen = aEndBit - aStartBit + 1;
        QBitArray ret(bitLen);

        for(int iBit=0; iBit<bitLen; iBit++)
        {
            ret.setBit(iBit, aContent.at(aStartBit+iBit));
        }

        return ret;
    }

    //FIXME: make this funciton more similar as hexToNipple
    QString nippleToHex(quint8 aNipple)
    {
        switch( aNipple & 0x0F )
        {
            case 0x0: return "0";
            case 0x1: return "1";
            case 0x2: return "2";
            case 0x3: return "3";
            case 0x4: return "4";
            case 0x5: return "5";
            case 0x6: return "6";
            case 0x7: return "7";
            case 0x8: return "8";
            case 0x9: return "9";
            case 0xa: return "A";
            case 0xb: return "B";
            case 0xc: return "C";
            case 0xd: return "D";
            case 0xe: return "E";
            case 0xf: return "F";
        }

        return "0";
    }

    quint8 hexToNipple(char aNipple, bool& isValid)
    {
        isValid = true;

        switch( aNipple )
        {
            case '0': return 0x0;
            case '1': return 0x1;
            case '2': return 0x2;
            case '3': return 0x3;
            case '4': return 0x4;
            case '5': return 0x5;
            case '6': return 0x6;
            case '7': return 0x7;
            case '8': return 0x8;
            case '9': return 0x9;
            case 'A': return 0xa;
            case 'B': return 0xb;
            case 'C': return 0xc;
            case 'D': return 0xd;
            case 'E': return 0xe;
            case 'F': return 0xf;
            default:
                isValid = false;
                return 0x0;
        }
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

    QBitArray & 	operator<< ( QBitArray & out, shift_t ba )
    {
        if( ba.resize ) {
            out.resize(out.size()+1);
            out<<ba.shifts;
            return out;
        }
        else {
            return out<<ba.shifts;
        }
    }

    QBitArray & 	operator>> ( QBitArray & in, shift_t  ba )
    {
        if( ba.resize ) {
            in>>ba.shifts;
            if( in.size() > 0)
                in.resize(in.size()-1);
            return in;
        }
        else {
            return in<<ba.shifts;
        }
    }

   QBitArray mirror(const QBitArray& in)
   {
       QBitArray ret(in);

       for(int i=0; i<ret.size()/2; i++) {
           int startPos = i;
           int endPos   = ret.size()-1-i;

           bool leftVal  = ret.testBit(startPos);
           ret.setBit(startPos, ret.testBit(endPos));
           ret.setBit(endPos, leftVal);
       }

       return ret;
   }

   QBitArray invert(const QBitArray& in)
   {
       QBitArray ret(in.size());

       for(int i=0; i<in.size(); i++) {
           ret.setBit(i, !in.testBit(i));
       }

       return ret;
   }



}
