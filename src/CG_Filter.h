#ifndef _CG_FILTER_
#define _CG_FILTER_

#include <vector>


template<class __T1, class __T2>
class CGFilter_Pair
{
public:
    CGFilter_Pair(__T1 value_1, __T2 value_2) : value_1(value_1), value_2(value_2) {};

    __T1 value_1;
    __T2 value_2;
};


template<class __T1, class __T2>
class CGFilter
{
    std::vector<CGFilter_Pair<__T1, __T2>> filter_;

public:
    CGFilter() {};

    bool addFilter(__T1 value_1, __T2 value_2);
    bool addFilter(CGFilter_Pair<__T1, __T2> value);

    bool removeFilter(__T1 value_1, __T2 value_2);
    bool removeFilter(CGFilter_Pair<__T1, __T2> value);

    void clear();

    bool check(__T1 value_1, __T2 value_2) const;
    bool check(CGFilter_Pair<__T1, __T2> value) const;

};


#include "CG_Filter.inl"

#endif
