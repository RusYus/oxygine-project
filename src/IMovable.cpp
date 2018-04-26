#include <iostream>
#include "ICarrier.hpp"

IMovable::IMovable(const std::shared_ptr<ICollisionManager>& a_Manager)
    : m_CollisionNormal(0, 0)
    , m_Rays(std::make_shared<std::vector<Collision::Ray>>())
    , m_Carrier(nullptr)
    , m_Level(0)
{
    a_Manager->AddBody(this);
}

IMovable::~IMovable()
{
    m_Rays->clear();
}

void IMovable::SetDirection(const Service::Vector2L& a_NewDirection)
{
    m_Direction = a_NewDirection;
    UpdateRays();
}

void IMovable::AddDirection(const Service::Vector2L& a_NewDirection)
{
    m_Direction += a_NewDirection;
    UpdateRays();
}

std::shared_ptr<std::vector<Collision::Ray>> IMovable::GetRays() const
{
    return m_Rays;
}

Service::Vector2L IMovable::GetDirection() const
{
    return m_Direction;
}

Service::Normal2 IMovable::GetCollisionNormal() const
{
    return m_CollisionNormal;
}

void IMovable::AddCollisionNormal(const Collision::CollisionInfo& a_Sides)
{
    if (a_Sides.Down)
    {
        m_CollisionNormal.y = -1;
    }

    if (a_Sides.Up)
    {
        m_CollisionNormal.y = 1;
    }

    if (a_Sides.Right)
    {
        m_CollisionNormal.x = 1;
    }

    if (a_Sides.Left)
    {
        m_CollisionNormal.x = -1;
    }
}

void IMovable::ResetCollisionNormal(const Collision::CollisionInfo& a_Sides)
{
    m_CollisionNormal.setZero();
    AddCollisionNormal(a_Sides);
}

void IMovable::SetPosition()
{
    Service::Vector2L newPos = GetPosition() + m_Direction;

    m_Position.set(newPos.x, newPos.y);

    for(auto& ray : *m_Rays)
    {
        ray.Original += m_Direction;
    }

    m_View->setPosition(Service::Convert(newPos));
}

void IMovable::UpdateRays()
{
    //std::cout << "Id:" << GetId() << std::endl;
    for(auto& ray : *m_Rays)
    {
        ray.Destination = ray.Original;

        switch (ray.Direction)
        {
            case Collision::RayDirection::Up:
                if (m_Direction.y < 0)
                {
                    ray.Destination.set(ray.Original.x, ray.Original.y + m_Direction.y);/* = Service::Vector2L(ray.Original.x, ray.Original.y + m_Direction.y);*/
                }
                // std::cout << "Up:  Original   : " << ray.Original.x << ":" << ray.Original.y << std::endl;
                // std::cout << "     Destination: " << ray.Destination.x << ":" << ray.Destination.y << std::endl;
                break;
            case Collision::RayDirection::Down:
                if (m_Direction.y > 0)
                {
                    ray.Destination.set(ray.Original.x, ray.Original.y + m_Direction.y);/* = Service::Vector2L(ray.Original.x, ray.Original.y + m_Direction.y);*/
                }
                // std::cout << "Down:  Original : " << ray.Original.x << ":" << ray.Original.y << std::endl;
                // std::cout << "     Destination: " << ray.Destination.x << ":" << ray.Destination.y << std::endl;
                break;
            case Collision::RayDirection::Right:
                if (m_Direction.x > 0)
                {
                    ray.Destination.set(ray.Original.x + m_Direction.x, ray.Original.y);/* = Service::Vector2L(ray.Original.x + m_Direction.x, ray.Original.y);*/
                }
                // std::cout << "Right:  Original: " << ray.Original.x << ":" << ray.Original.y << std::endl;
                // std::cout << "     Destination: " << ray.Destination.x << ":" << ray.Destination.y << std::endl;
                break;

            case Collision::RayDirection::Left:
                if (m_Direction.x < 0)
                {
                    ray.Destination.set(ray.Original.x + m_Direction.x, ray.Original.y);/* = Service::Vector2L(ray.Original.x + m_Direction.x, ray.Original.y);*/
                }
                // std::cout << "Left:  Original : " << ray.Original.x << ":" << ray.Original.y << std::endl;
                // std::cout << "     Destination: " << ray.Destination.x << ":" << ray.Destination.y << std::endl;
                break;
            case Collision::RayDirection::UpLeft:
                if (m_Direction.x < 0 && m_Direction.y < 0)
                {
                    ray.Destination = Service::Vector2L(ray.Original + m_Direction);
                }
                // std::cout << "UpLeft:  Original: " << ray.Original.x << ":" << ray.Original.y << std::endl;
                // std::cout << "     Destination : " << ray.Destination.x << ":" << ray.Destination.y << std::endl;
                break;
            case Collision::RayDirection::UpRight:
                if (m_Direction.x > 0 && m_Direction.y < 0)
                {
                    ray.Destination = Service::Vector2L(ray.Original + m_Direction);
                }
                // std::cout << "UpRight:  Original: " << ray.Original.x << ":" << ray.Original.y << std::endl;
                // std::cout << "     Destination  : " << ray.Destination.x << ":" << ray.Destination.y << std::endl;
                break;
            case Collision::RayDirection::DownLeft:
                if (m_Direction.x < 0 && m_Direction.y > 0)
                {
                    ray.Destination = Service::Vector2L(ray.Original + m_Direction);
                }
                // std::cout << "DownLeft:  Original: " << ray.Original.x << ":" << ray.Original.y << std::endl;
                // std::cout << "     Destination   : " << ray.Destination.x << ":" << ray.Destination.y << std::endl;
                break;
            case Collision::RayDirection::DownRight:
                if (m_Direction.x > 0 && m_Direction.y > 0)
                {
                    ray.Destination = Service::Vector2L(ray.Original + m_Direction);
                }
                // std::cout << "DownRight:  Original: " << ray.Original.x << ":" << ray.Original.y << std::endl;
                // std::cout << "     Destination    : " << ray.Destination.x << ":" << ray.Destination.y << std::endl;
                break;
        }
    }
}

void IMovable::SetRays()
{
    m_Rays->clear();

    int actualIntervalsNumber = static_cast<int>(std::ceil(GetWidthRaw() / Service::Constants::RAYCAST_INTERVAL));
    long long actualIntervalLength = std::round(GetWidthRaw() / actualIntervalsNumber);
    actualIntervalsNumber--;
    for (int i = 0; i < actualIntervalsNumber + 2; ++i)
    {
        // Bottom
        m_Rays->emplace_back(Collision::Ray(Service::Vector2L(GetX() + i * actualIntervalLength, GetY() + GetHeightRaw()),
                                          Service::Vector2L(GetX() + i * actualIntervalLength, GetY() + GetHeightRaw()),
                                          Collision::RayDirection::Down));
        // Top
        m_Rays->emplace_back(Collision::Ray(Service::Vector2L(GetX() + i * actualIntervalLength, GetY()),
                                          Service::Vector2L(GetX() + i * actualIntervalLength, GetY()),
                                          Collision::RayDirection::Up));
    }

    actualIntervalsNumber = static_cast<int>(std::ceil(GetHeightRaw() / Service::Constants::RAYCAST_INTERVAL));
    actualIntervalLength = std::round(GetHeightRaw() / actualIntervalsNumber);
    actualIntervalsNumber--;
    for (int i = 0; i < actualIntervalsNumber + 2; ++i)
    {
        // Right
        m_Rays->emplace_back(Collision::Ray(Service::Vector2L(GetX() + GetWidthRaw(), GetY() + i * actualIntervalLength),
                                          Service::Vector2L(GetX() + GetWidthRaw(), GetY() + i * actualIntervalLength),
                                          Collision::RayDirection::Right));
        // Left
        m_Rays->emplace_back(Collision::Ray(Service::Vector2L(GetX(), GetY() + i * actualIntervalLength),
                                          Service::Vector2L(GetX(), GetY() + i * actualIntervalLength),
                                          Collision::RayDirection::Left));
    }

    // UpLeft diagonal.
    m_Rays->emplace_back(Collision::Ray(Service::Vector2L(GetX(), GetY()),
                                       Service::Vector2L(GetX(), GetY()),
                                       Collision::RayDirection::UpLeft));
    // UpRight diagonal.
    m_Rays->emplace_back(Collision::Ray(Service::Vector2L(GetX() + GetWidthRaw(), GetY()),
                                       Service::Vector2L(GetX() + GetWidthRaw(), GetY()),
                                       Collision::RayDirection::UpRight));
    // DownLeft diagonal.
    m_Rays->emplace_back(Collision::Ray(Service::Vector2L(GetX(), GetY() + GetHeightRaw()),
                                       Service::Vector2L(GetX(), GetY() + GetHeightRaw()),
                                       Collision::RayDirection::DownLeft));
    // DownRight diagonal.
    m_Rays->emplace_back(Collision::Ray(Service::Vector2L(GetX() + GetWidthRaw(), GetY() + GetHeightRaw()),
                                       Service::Vector2L(GetX() + GetWidthRaw(), GetY() + (GetHeightRaw())),
                                       Collision::RayDirection::DownRight));
}

void IMovable::DetachFromCarrier()
{
    m_Carrier = nullptr;
    assert(m_Level > 0);
    m_Level--;
}

void IMovable::AttachToCarrier(ICarrier* a_Carrier)
{
    m_Carrier = a_Carrier;
    m_Level = a_Carrier->GetLevel() + 1;
}

bool IMovable::IsAttachToAnyCarrier()
{
    return m_Carrier != nullptr;
}

bool IMovable::IsAttachToCarrier(Basis::BasisObject::TId a_Id)
{
    return m_Carrier != nullptr && m_Carrier->GetId() == a_Id;
}

IMovable::TLevel IMovable::GetLevel() const
{
    return m_Level;
}
