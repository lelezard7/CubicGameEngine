#include "CG_Filter.h"


template<class __T1, class __T2>
bool
CGFilter<__T1, __T2>::
addFilter(__T1 value_1, __T2 value_2)
{
    for (size_t i = 0; i < filter_.size(); ++i) {
        if (filter_[i].value_1 == value_1 && filter_[i].value_2 == value_2)
            return false;
    }

    filter_.push_back(CGFilter_Pair<__T1, __T2>(value_1, value_2));
    return true;
}

template<class __T1, class __T2>
bool
CGFilter<__T1, __T2>::
addFilter(CGFilter_Pair<__T1, __T2> value)
{
    return addFilter(value.value_1, value.value_2);
}

template<class __T1, class __T2>
bool
CGFilter<__T1, __T2>::
removeFilter(__T1 value_1, __T2 value_2)
{
    for (auto it = filter_.begin(); it != filter_.end(); ++it) {
        if (it->value_1 == value_1 && it->value_2 == value_2) {
            filter_.erase(it);
            return true;
        }
    }

    return false;
}

template<class __T1, class __T2>
bool
CGFilter<__T1, __T2>::
removeFilter(CGFilter_Pair<__T1, __T2> value)
{
    return removeFilter(value.value_1, value.value_2);
}

template<class __T1, class __T2>
void
CGFilter<__T1, __T2>::
clear()
{
    filter_.clear();
}

template<class __T1, class __T2>
bool
CGFilter<__T1, __T2>::
check(__T1 value_1, __T2 value_2) const
{
    for (size_t i = 0; i < filter_.size(); ++i) {
        if (filter_[i].value_1 == value_1 && filter_[i].value_2 == value_2) {
            return true;
        }
    }

    return false;
}

template<class __T1, class __T2>
bool
CGFilter<__T1, __T2>::
check(CGFilter_Pair<__T1, __T2> value) const
{
    return check(value.value_1, value.value_2);
}
