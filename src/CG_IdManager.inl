#include "CG_IdManager.h"


template<class __T>
CGIdManager<__T>::
CGIdManager()
{
    startId_ = (__T)0;
    maxId_ = startId_;
}

template<class __T>
CGIdManager<__T>::
CGIdManager(__T startId)
{
    startId_ = startId;
    maxId_ = startId;
}

template<class __T>
__T
CGIdManager<__T>::
getFreeId()
{
    __T _freeId;

    if (freeId_.size() != 0) {
        auto _it = freeId_.begin();
        _freeId = *_it;
        freeId_.erase(_it);
        return _freeId;
    }

    _freeId = maxId_;
    maxId_++;
    return _freeId;
}

template<class __T>
void
CGIdManager<__T>::
freeId(__T id)
{
    if (id >= maxId_)
        return;

    for (size_t i = 0; i < freeId_.size(); ++i) {
        if (freeId_[i] == id)
            return;
    }

    if (maxId_ - 1 == id) {
        maxId_--;
        return;
    }

    freeId_.push_back(id);
}

template<class __T>
void
CGIdManager<__T>::
freeAll()
{
    freeId_.clear();
    maxId_ = startId_;
}

template<class __T>
bool
CGIdManager<__T>::
checkId(__T id) const
{
    if (id >= maxId_)
        return false;

    for (size_t i = 0; i < freeId_.size(); ++i) {
        if (freeId_[i] == id)
            return false;
    }

    return true;
}
