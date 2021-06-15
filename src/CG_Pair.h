#ifndef _CG_PAIR_
#define _CG_PAIR_


template <class __T1, class __T2>
class CGPair
{
public:
    CGPair() {};
    CGPair(const CGPair& other) : first(other.first), second(other.second) {};
    CGPair (__T1 first, __T2 second) : first(first), second(second) {};

    __T1 first;
    __T2 second;

    void operator=(const CGPair& other) {
        first = other.first;
        second = other.second;
    };

};

#endif
