#pragma once

#include <vector>

#include "math/Vector2.h"

#include "CollisionInfo.hpp"
#include "Constants.hpp"
#include "IMovable.hpp"
#include "Utils.hpp"

class ICarrier : public virtual IMovable
{
public:
    using TLevel = unsigned;
public:
    ICarrier(const std::shared_ptr<ICollisionManager>&);

    virtual ~ICarrier();

    void SetPosition() override;
    void AttachToCarrier(ICarrier* a_Carrier) override;
    void DetachFromCarrier() override;

    std::shared_ptr<std::vector<Collision::Ray>> GetCarrierRays() const;
    void AddPassenger(IMovable* a_Body);
    void RemovePassenger(IMovable* a_Body);
    bool IsPassengerExists(IMovable* a_Body);
    TLevel GetCarrierLevel() const;

protected:
    void MovePassengers();
    void PassengersCheckCollisions();
    void ClearPassengers();
    void UpdateRays() override;
    void SetRays() override;

protected:
    const float m_RayLength = Service::Constants::CARRIER_RAY_LENGTH;
    TLevel m_Level;
    std::shared_ptr<std::vector<Collision::Ray>> m_CarrierRays;
    std::unique_ptr<std::vector<IMovable*>> m_Passengers;
};
