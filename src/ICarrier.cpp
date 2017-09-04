#include <iostream>
#include "ICarrier.hpp"

ICarrier::ICarrier()
    : m_CarrierRays(std::make_shared<std::vector<Collision::Ray>>())
{}

ICarrier::~ICarrier()
{
    m_CarrierRays->clear();
}

std::shared_ptr<std::vector<Collision::Ray>> ICarrier::GetCarrierRays() const
{
    return m_CarrierRays;
}

void ICarrier::AddPassenger(IMovable* a_Body)
{
    if (!a_Body || std::find(m_Passengers.cbegin(), m_Passengers.cend(), a_Body) != m_Passengers.cend())
    {
        std::cout << "Can't add passenger: NULL or already exists!" << std::endl;
        return;
    }

    m_Passengers.push_back(a_Body);
}

void ICarrier::ClearPassengers()
{
    m_Passengers.clear();
}

void ICarrier::SetPosition()
{
    IMovable::SetPosition();

    for(auto& ray : *m_CarrierRays)
    {
        ray.Original += m_Direction;
    }

    for (auto& pass : m_Passengers)
    {
        if (!pass)
        {
            std::cout << "Passenger is NULL!" << std::endl;
            continue;
        }

        // Move.
//        pass.SetDirection();
    }
}

void ICarrier::UpdateRays()
{
    IMovable::UpdateRays();

    for(auto& ray : *m_CarrierRays)
    {
        ray.Destination = oxygine::Vector2(ray.Original.x, ray.Original.y - m_RayLength);
    }
}

void ICarrier::SetRays()
{
    IMovable::SetRays();

    m_CarrierRays->clear();

    int actualIntervalsNumber = static_cast<int>(std::ceil(GetWidth() / Service::Constants::RAYCAST_INTERVAL));
    float actualIntervalLength = GetWidth() / actualIntervalsNumber;
    actualIntervalsNumber--;
    for (int i = 0; i < actualIntervalsNumber + 2; ++i)
    {
        // Top
        m_CarrierRays->emplace_back(Collision::Ray(oxygine::Vector2(GetX() + i * actualIntervalLength, GetY()),
                                          oxygine::Vector2(GetX() + i * actualIntervalLength, GetY()),
                                          Collision::RayDirection::Up));
    }
}
