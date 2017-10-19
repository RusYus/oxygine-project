#include <iostream>
#include "ICarrier.hpp"

ICarrier::ICarrier(const std::shared_ptr<ICollisionManager>& a_Manager)
    : IMovable(a_Manager)
    , m_CarrierRays(std::make_shared<std::vector<Collision::Ray>>())
    , m_Passengers(std::make_unique<std::vector<IMovable*>>())
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
    assert(m_Passengers != nullptr);

    if (!a_Body || std::find(m_Passengers->cbegin(), m_Passengers->cend(), a_Body) != m_Passengers->cend())
    {
        std::cout << "Can't add passenger: NULL or already exists!" << std::endl;
        return;
    }

    a_Body->CarrierId = m_Id;
    m_Passengers->push_back(a_Body);
//    std::cout << "Adding passenger" << std::endl;
}

void ICarrier::ClearPassengers()
{
    assert(m_Passengers != nullptr);
    for (auto& passenger : *m_Passengers)
    {
        passenger->CarrierId = Service::IdGenerator::UnknownId;
    }
    m_Passengers->clear();
//    std::cout << "Clear all passengers" << std::endl;
}

void ICarrier::SetPosition()
{
    assert(m_Passengers != nullptr);

    IMovable::SetPosition();

    for(auto& ray : *m_CarrierRays)
    {
        ray.Original += m_Direction;
    }
}

void ICarrier::UpdateRays()
{
    IMovable::UpdateRays();

    for(auto& ray : *m_CarrierRays)
    {
        ray.Destination = Service::Vector2L(ray.Original.x, ray.Original.y - m_RayLength);
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
        m_CarrierRays->emplace_back(Collision::Ray(Service::Vector2L(GetX() + i * actualIntervalLength, GetY()),
                                          Service::Vector2L(GetX() + i * actualIntervalLength, GetY()),
                                          Collision::RayDirection::Up));
    }
}

void ICarrier::MovePassengers()
{
    for (auto& passenger : *m_Passengers)
    {
        if (!passenger)
        {
            std::cout << "Moving passengers: passenger is NULL!" << std::endl;
            continue;
        }

        passenger->AddDirection(m_Direction);
        passenger->AddCollisionNormal(Collision::CollisionInfoDown);
    }
}

void ICarrier::PassengersCheckCollisions()
{
    for (auto& passenger : *m_Passengers)
    {
        if (!passenger)
        {
            std::cout << "Moving passengers: passenger is NULL!" << std::endl;
            continue;
        }

        passenger->CheckCollisions();
    }
}
