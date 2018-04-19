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
    ICarrier(const std::shared_ptr<ICollisionManager>&);

    virtual ~ICarrier();

    void SetPosition() override;
    std::shared_ptr<std::vector<Collision::Ray>> GetCarrierRays() const;
    void AddPassenger(IMovable* a_Body);
    void RemovePassenger(IMovable* a_Body);
    bool IsPassengerExists(IMovable* a_Body);

protected:
    void MovePassengers();
    void PassengersCheckCollisions();
    void ClearPassengers();
    void UpdateRays() override;
    void SetRays() override;

protected:
    const float m_RayLength = Service::Constants::CARRIER_RAY_LENGTH;
    std::shared_ptr<std::vector<Collision::Ray>> m_CarrierRays;
    std::unique_ptr<std::vector<IMovable*>> m_Passengers;
};
