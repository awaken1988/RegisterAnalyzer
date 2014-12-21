#include "registerfield.h"

RegisterField::RegisterField(int aStart, int aEnd)
    : m_start(aStart), m_end(aEnd)
{
    if(m_end > m_start)
        throw QString("endpos < startpos");
    if( 0 > m_start || 0 > m_end)
        throw QString("startpos or endpos < 0");
}

int RegisterField::bitlength() const
{
    return m_end - m_start + 1;
}

template<typename T>
T RegisterField::getValue(const QBitArray& aContent)            //TODO: check if t is int long...
{
    T retval=0;

    for(int iBit=m_start; iBit<=m_end; iBit++)
    {
        retval |= aContent.at(iBit)<<(iBit-m_start);
    }

    return retval;
}
