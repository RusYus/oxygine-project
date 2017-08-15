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
    IMovable()
        : m_CollisionNormal(0, 0)
    {}

    virtual ~IMovable() = default;

    virtual void SetDirection(const oxygine::Vector2&);
    virtual std::vector<Collision::Ray>& GetRays();
    virtual oxygine::Vector2 GetDirection() const;
    virtual void SetCollisionNormal(const oxygine::Vector2);
    virtual void ResetCollisionNormal(const Collision::CollisionInfo&);
    virtual void SetPosition();

protected:
    void UpdateRays(bool aOriginal);
    void SetRays();

protected:
    oxygine::Vector2 m_Direction;
    Service::Normal2 m_CollisionNormal;
    // TODO : store shared_ptr to vector
    std::vector<Collision::Ray> m_Rays;
    const int m_MaxSpeed = Service::Constants::MAX_SPEED;
};
