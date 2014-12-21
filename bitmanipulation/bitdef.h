#ifndef BITDEF_H
#define BITDEF_H


namespace NsBitDef
{

    constexpr int BIT_PER_BYTE      = 8;
    constexpr int BIT_PER_NIPPLE    = 4;

    template<typename T> struct bit_traits
    {
        static const int size = sizeof(T) * BIT_PER_BYTE;
    };

}



#endif // BITDEF_H
