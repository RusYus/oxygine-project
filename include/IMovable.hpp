#pragma once

#include <vector>

#include "math/Vector2.h"

#include "CollisionInfo.hpp"
#include "Constants.hpp"
#include "Utils.hpp"

class IMovable
{
public:
    void SetCollisionNormal(const oxygine::Vector2);
    void SetDebugDraw(bool);
    bool GetDebugDraw() const;

    ~IMovable() = default;
protected:
    oxygine::Vector2 m_Direction;
    Service::Normal2 m_CollisionNormal;
    std::vector<Collision::Ray> m_Rays;
    bool m_DebugDraw = false;
    const int m_MaxSpeed = Service::Constants::MAX_SPEED;
};
