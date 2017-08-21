#pragma once

#include <vector>

#include "math/Vector2.h"

#include "CollisionInfo.hpp"
#include "Constants.hpp"
#include "IDrawable.hpp"
#include "Utils.hpp"

class IMovable : public virtual IDrawable
{
public:
    IMovable();

    virtual ~IMovable();

    virtual void SetDirection(const oxygine::Vector2&, bool /*a_SetExact*/ = false);
    virtual std::shared_ptr<std::vector<Collision::Ray>> GetRays() const;
    virtual oxygine::Vector2 GetDirection() const;
    virtual void ResetCollisionNormal(const Collision::CollisionInfo&);
    virtual void SetPosition();

protected:
    virtual void UpdateRays();
    virtual void SetRays();

protected:
    oxygine::Vector2 m_Direction;
    Service::Normal2 m_CollisionNormal;
    std::shared_ptr<std::vector<Collision::Ray>> m_Rays;
    const int m_MaxSpeed = Service::Constants::MAX_SPEED;
};
