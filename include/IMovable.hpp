#pragma once

#include <vector>

#include "math/Vector2.h"

#include "ICollisionManager.hpp"
#include "CollisionInfo.hpp"
#include "Constants.hpp"
#include "IDrawable.hpp"
#include "Utils.hpp"

class IMovable : public virtual IDrawable
{
public:
    IMovable();

    virtual ~IMovable();

    void BindCollisionManager(const std::shared_ptr<ICollisionManager>&);

    virtual void SetDirection(const oxygine::Vector2&);
    virtual void AddDirection(const oxygine::Vector2&);
    virtual std::shared_ptr<std::vector<Collision::Ray>> GetRays() const;
    virtual oxygine::Vector2 GetDirection() const;
    virtual Service::Normal2 GetCollisionNormal() const;
    virtual void SetCollisionNormal(const Service::Normal2&);
    virtual void ResetCollisionNormal(const Collision::CollisionInfo&);
    virtual void SetPosition();
    virtual void CheckCollisions();

protected:
    virtual void UpdateRays();
    virtual void SetRays();

protected:
//    oxygine::Vector2 m_Direction;
    Model::Velocity m_Direction;
    Service::Normal2 m_CollisionNormal;
    std::shared_ptr<ICollisionManager> m_CollisionManager;
    std::shared_ptr<std::vector<Collision::Ray>> m_Rays;
    const int m_MaxSpeed = Service::Constants::MAX_SPEED;
};
