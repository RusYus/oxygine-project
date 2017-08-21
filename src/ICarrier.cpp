#include <iostream>
#include "ICarrier.hpp"

ICarrier::ICarrier()
    : m_CarrierRays(std::make_shared<std::vector<Collision::Ray>>())
{}

ICarrier::~ICarrier()
{
    m_CarrierRays->clear();
}

std::shared_ptr<std::vector<Collision::Ray>> ICarrier::GetRays() const
{
    return m_CarrierRays;
}

void ICarrier::SetPosition()
{
    oxygine::Vector2 newPos = GetPosition() + m_Direction;

    for(auto& ray : *m_CarrierRays)
    {
        ray.Original += m_Direction;
    }

    m_View->setPosition(newPos);
}

void ICarrier::UpdateRays()
{
    for(auto& ray : *m_CarrierRays)
    {
        ray.Destination = oxygine::Vector2(ray.Original.x, ray.Original.y - m_RayLength);
    }
}

void ICarrier::SetRays()
{
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
