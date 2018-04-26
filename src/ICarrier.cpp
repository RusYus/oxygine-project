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

    if (!a_Body)
    {
        return;
    }

    auto passengerIt = std::find(m_Passengers->cbegin(), m_Passengers->cend(), a_Body);
    if (passengerIt != m_Passengers->cend())
    {
        MovePassenger(*passengerIt);
        return;
    }

    a_Body->AttachToCarrier(this);
    MovePassenger(a_Body);
    m_Passengers->push_back(a_Body);
//    std::cout << "Adding passenger" << std::endl;
}

void ICarrier::RemovePassenger(IMovable* a_Body)
{
    assert(m_Passengers != nullptr);
    auto passengerIt = std::find_if(m_Passengers->begin(), m_Passengers->end(), [&a_Body] (IMovable* a_Item) { return a_Item == a_Body; });
    (*passengerIt)->DetachFromCarrier();
    m_Passengers->erase(passengerIt);
}

bool ICarrier::IsPassengerExists(IMovable* a_Body)
{
    assert(m_Passengers != nullptr);
    auto passengerIt = std::find_if(m_Passengers->begin(), m_Passengers->end(), [&a_Body] (IMovable* a_Item) { return a_Item == a_Body; });
    return passengerIt == m_Passengers->end() ? false : true;
}

void ICarrier::ClearPassengers()
{
    assert(m_Passengers != nullptr);
    for (auto& passenger : *m_Passengers)
    {
        passenger->DetachFromCarrier();
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

void ICarrier::MovePassenger(IMovable* a_Passenger)
{
    assert(a_Passenger && "Moving passenger: passenger is NULL!");
    a_Passenger->AddDirection(m_Direction);
    a_Passenger->AddCollisionNormal(Collision::CollisionInfoDown);
}
