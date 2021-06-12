#ifndef _CG_PAIR_
#define _CG_PAIR_


template <class __T1, class __T2>
struct CGPair
{
    CGPair() {};
    CGPair (__T1 first, __T2 second) : first(first), second(second) {};

    __T1 first;
    __T2 second;
};

#endif
