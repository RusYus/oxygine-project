#pragma once

#include "Actor.h"

#include "Utils.hpp"

namespace Basis
{

class BasisObject
{
public:
    using TId = Service::IdGenerator::TId;

    BasisObject();

    virtual ~BasisObject() = default;

    virtual inline Service::TCoordinate GetX() const
    {
        return m_Position.x;
    }

    virtual inline Service::TCoordinate GetY() const
    {
        return m_Position.y;
    }

    virtual Service::TCoordinate GetWidth() const = 0;
    virtual Service::TCoordinate GetHeight() const = 0;
    virtual Service::Vector2L GetPosition() const
    {
        return m_Position;
    }

    inline TId GetId() const
    {
        return m_Id;
    }

protected:
     TId m_Id;
     Service::Vector2L m_Position;
};

}
