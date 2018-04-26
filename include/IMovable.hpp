#pragma once

#include <vector>

#include "math/Vector2.h"

#include "ICollisionManager.hpp"
#include "CollisionInfo.hpp"
#include "Constants.hpp"
#include "IDrawable.hpp"
#include "Utils.hpp"
#include "Velocity.hpp"

// Shit forward declaration.
class ICarrier;

class IMovable : public virtual IDrawable
{
public:
    using TLevel = unsigned;

public:
    IMovable(const std::shared_ptr<ICollisionManager>&);

    virtual ~IMovable();

    virtual void SetDirection(const Service::Vector2L&);
    virtual void AddDirection(const Service::Vector2L&);
    virtual std::shared_ptr<std::vector<Collision::Ray>> GetRays() const;
    virtual Service::Vector2L GetDirection() const;
    virtual Service::Normal2 GetCollisionNormal() const;
    virtual void AddCollisionNormal(const Collision::CollisionInfo&);
    virtual void ResetCollisionNormal(const Collision::CollisionInfo&);
    virtual void SetPosition();
    virtual void AttachToCarrier(ICarrier* a_Carrier);
    virtual void DetachFromCarrier();
    virtual bool IsAttachToAnyCarrier();
    virtual bool IsAttachToCarrier(Basis::BasisObject::TId a_Id);
    TLevel GetLevel() const;

protected:
    virtual void UpdateRays();
    virtual void SetRays();

protected:
    Service::Vector2L m_Direction;
    Service::Normal2 m_CollisionNormal;
    std::shared_ptr<std::vector<Collision::Ray>> m_Rays;
    const int m_MaxSpeed = Service::Constants::MAX_SPEED;
    ICarrier* m_Carrier;
    TLevel m_Level;
};
