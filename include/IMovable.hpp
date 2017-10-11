#pragma once

#include <vector>

#include "math/Vector2.h"

#include "ICollisionManager.hpp"
#include "CollisionInfo.hpp"
#include "Constants.hpp"
#include "IDrawable.hpp"
#include "Utils.hpp"
#include "Velocity.hpp"

class IMovable : public virtual IDrawable
{
public:
    IMovable();

    virtual ~IMovable();

    void BindCollisionManager(const std::shared_ptr<ICollisionManager>&);

    virtual void SetDirection(const Service::Vector2L&);
    virtual void AddDirection(const Service::Vector2L&);
    virtual void AddDirectionX(Service::TCoordinate);
    virtual void AddDirectionY(Service::TCoordinate);
    virtual std::shared_ptr<std::vector<Collision::Ray>> GetRays() const;
    virtual Service::Vector2L GetDirection() const;
    virtual Service::Normal2 GetCollisionNormal() const;
    virtual void SetCollisionNormal(const Service::Normal2&);
    virtual void ResetCollisionNormal(const Collision::CollisionInfo&);
    virtual void SetPosition();
    virtual void CheckCollisions();

protected:
    virtual void UpdateRays();
    virtual void SetRays();

public:
    Basis::BasisObject::TId CarrierId;

protected:
    Service::Vector2L m_Direction;
    Service::Normal2 m_CollisionNormal;
    std::shared_ptr<ICollisionManager> m_CollisionManager;
    std::shared_ptr<std::vector<Collision::Ray>> m_Rays;
    const int m_MaxSpeed = Service::Constants::MAX_SPEED;
};
