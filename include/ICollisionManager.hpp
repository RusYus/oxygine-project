#pragma once

#include "BasisObject.hpp"

class ICollisionManager
{
public:
    ICollisionManager() = default;

    virtual ~ICollisionManager() = default;

    virtual void AddBody(Basis::BasisObject*) = 0;
    virtual void PrintCarrierId() = 0;
    virtual void CheckCollisions(Basis::BasisObject::TId) = 0;
};
