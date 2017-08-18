#pragma once

#include "Actor.h"

#include "Utils.hpp"

namespace Basis
{

class BasisObject
{
public:
    BasisObject()
    {
        m_Id = Service::IdGenerator::GetNextId();
    }

    virtual ~BasisObject() = default;

    virtual inline float GetX() const = 0;
    virtual inline float GetY() const = 0;
    virtual inline float GetWidth() const = 0;
    virtual inline float GetHeight() const = 0;
    virtual inline oxygine::Vector2 GetPosition() const = 0;

    inline Service::IdGenerator::TId GetId() const
    {
        return m_Id;
    }

protected:
    Service::IdGenerator::TId m_Id;
};

}
