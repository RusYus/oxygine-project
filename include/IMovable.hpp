#pragma once

#include <vector>

#include "math/Vector2.h"

#include "BasisObject.hpp"
#include "CollisionInfo.hpp"
#include "Constants.hpp"
#include "Utils.hpp"

class IMovable : public Basis::BasisObject
{
public:
    IMovable()
        : m_CollisionNormal(0, 0)
    {}

    virtual ~IMovable() = default;

    virtual void SetDirection(const oxygine::Vector2&) = 0;
    virtual oxygine::Vector2 GetRayOriginal() const = 0;
    virtual std::vector<Collision::Ray>& GetRays() = 0;
    virtual oxygine::Vector2 GetRayDestination() const = 0;
    virtual oxygine::Vector2 GetDirection() const = 0;
    virtual Service::Normal2 GetCollisionNormal() const = 0;
    virtual void SetCollisionNormal(const oxygine::Vector2) = 0;
    virtual void ResetCollisionNormal(const Collision::CollisionInfo&) = 0;

protected:
    oxygine::Vector2 m_Direction;
    Service::Normal2 m_CollisionNormal;
    std::vector<Collision::Ray> m_Rays;
    bool m_DebugDraw = false;
    const int m_MaxSpeed = Service::Constants::MAX_SPEED;
};
