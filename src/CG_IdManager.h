#ifndef _CG_ID_MANAGER_
#define _CG_ID_MANAGER_

#include <vector>


template<class __T>
class CGIdManager
{
    std::vector<__T> freeId_;
    __T maxId_;
    __T startId_;

public:
    CGIdManager();
    CGIdManager(__T startId);

    __T getFreeId();
    void freeId(__T id);
    void freeAll();

    bool checkId(__T id) const;

};


#include "CG_IdManager.inl"

#endif
