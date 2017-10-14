#pragma once

#include "BasisObject.hpp"

class ICollisionManager
{
public:
    ICollisionManager() = default;

    virtual ~ICollisionManager() = default;

    virtual void AddBody(Basis::BasisObject*) {}
    virtual void CheckCollisions(Basis::BasisObject::TId) {}
};
