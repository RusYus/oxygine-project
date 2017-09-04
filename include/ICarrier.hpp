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
    ICarrier();

    virtual ~ICarrier();

    std::shared_ptr<std::vector<Collision::Ray>> GetCarrierRays() const;
    void AddPassenger(IMovable* a_Body);
    void SetPosition() override;

protected:
    void ClearPassengers();
    void UpdateRays() override;
    void SetRays() override;

protected:
    const float m_RayLength = Service::Constants::CARRIER_RAY_LENGTH;
    std::shared_ptr<std::vector<Collision::Ray>> m_CarrierRays;
    std::unique_ptr<std::vector<IMovable*>> m_Passengers;
};
