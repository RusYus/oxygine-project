#pragma once
#include "Utils.hpp"

namespace Basis
{

class BasisObject
{
public:
    BasisObject()
    {
        mId = Service::IdGenerator::GetNextId();
    }

    virtual ~BasisObject() = default;

    int GetId() const
    {
        return mId;
    }

protected:
    int mId;
};

}
