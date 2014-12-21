#ifndef REGISTERFIELD_H
#define REGISTERFIELD_H

#include "bitdef.h"
#include <QtGlobal>
#include <QBitArray>
#include <QString>
#include <exception>



class RegisterField
{
public:
    RegisterField(int aStart, int aEnd);

    int bitlength() const;
    template<typename T> T getValue(const QBitArray& aContent);

protected:
    int m_start;
    int m_end;
};

template<typename T> void fillValue(QBitArray& aContent, T aValue)
{
    const int valSize = sizeof(aValue)*8;


    for(int iBit=0; iBit<sizeof(aValue)*8; iBit++)
    {

    }
}

#endif // REGISTERFIELD_H
