#include <iostream>
#include "IMovable.hpp"

IMovable::IMovable()
    : m_CollisionNormal(0, 0)
    , m_Rays(std::make_shared<std::vector<Collision::Ray>>())
{
}

void IMovable::BindCollisionManager(const std::shared_ptr<ICollisionManager>& a_Manager)
{
    assert(!m_CollisionManager);
    m_CollisionManager = std::shared_ptr<ICollisionManager>(a_Manager);
}

IMovable::~IMovable()
{
    m_Rays->clear();
}

void IMovable::SetDirection(const Service::Vector2L& aNewDirection)
{
    m_Direction = aNewDirection;
}

void IMovable::AddDirection(const Service::Vector2L& aNewDirection)
{
    m_Direction += aNewDirection;
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

void IMovable::SetCollisionNormal(const Service::Normal2& a_Normal)
{
    m_CollisionNormal = a_Normal;
}

void IMovable::ResetCollisionNormal(const Collision::CollisionInfo& a_Sides)
{
    m_CollisionNormal.setZero();

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
    std::cout << "Id:" << GetId() << std::endl;
    for(auto& ray : *m_Rays)
    {
        ray.Destination = ray.Original;

        switch (ray.Direction)
        {
            case Collision::RayDirection::Up:
                if (m_Direction.y < 0)
                {
                    ray.Destination = Service::Vector2L(ray.Original.x, ray.Original.y + m_Direction.y);
                }
                std::cout << "Up:  Original   : " << ray.Original.x << ":" << ray.Original.y << std::endl;
                std::cout << "     Destination: " << ray.Destination.x << ":" << ray.Destination.y << std::endl;
                break;
            case Collision::RayDirection::Down:
                if (m_Direction.y > 0)
                {
                    ray.Destination = Service::Vector2L(ray.Original.x, ray.Original.y + m_Direction.y);
                }
                std::cout << "Down:  Original : " << ray.Original.x << ":" << ray.Original.y << std::endl;
                std::cout << "     Destination: " << ray.Destination.x << ":" << ray.Destination.y << std::endl;
                break;
            case Collision::RayDirection::Right:
                if (m_Direction.x > 0)
                {
                    ray.Destination = Service::Vector2L(ray.Original.x + m_Direction.x, ray.Original.y);
                }
                std::cout << "Right:  Original: " << ray.Original.x << ":" << ray.Original.y << std::endl;
                std::cout << "     Destination: " << ray.Destination.x << ":" << ray.Destination.y << std::endl;
                break;

            case Collision::RayDirection::Left:
                if (m_Direction.x < 0)
                {
                    ray.Destination = Service::Vector2L(ray.Original.x + m_Direction.x, ray.Original.y);
                }
                std::cout << "Left:  Original : " << ray.Original.x << ":" << ray.Original.y << std::endl;
                std::cout << "     Destination: " << ray.Destination.x << ":" << ray.Destination.y << std::endl;
                break;
            case Collision::RayDirection::UpLeft:
                if (m_Direction.x < 0 && m_Direction.y < 0)
                {
                    ray.Destination = Service::Vector2L(ray.Original + m_Direction);
                }
                std::cout << "UpLeft:  Original: " << ray.Original.x << ":" << ray.Original.y << std::endl;
                std::cout << "     Destination : " << ray.Destination.x << ":" << ray.Destination.y << std::endl;
                break;
            case Collision::RayDirection::UpRight:
                if (m_Direction.x > 0 && m_Direction.y < 0)
                {
                    ray.Destination = Service::Vector2L(ray.Original + m_Direction);
                }
                std::cout << "UpRight:  Original: " << ray.Original.x << ":" << ray.Original.y << std::endl;
                std::cout << "     Destination  : " << ray.Destination.x << ":" << ray.Destination.y << std::endl;
                break;
            case Collision::RayDirection::DownLeft:
                if (m_Direction.x < 0 && m_Direction.y > 0)
                {
                    ray.Destination = Service::Vector2L(ray.Original + m_Direction);
                }
                std::cout << "DownLeft:  Original: " << ray.Original.x << ":" << ray.Original.y << std::endl;
                std::cout << "     Destination   : " << ray.Destination.x << ":" << ray.Destination.y << std::endl;
                break;
            case Collision::RayDirection::DownRight:
                if (m_Direction.x > 0 && m_Direction.y > 0)
                {
                    ray.Destination = Service::Vector2L(ray.Original + m_Direction);
                }
                std::cout << "DownRight:  Original: " << ray.Original.x << ":" << ray.Original.y << std::endl;
                std::cout << "     Destination    : " << ray.Destination.x << ":" << ray.Destination.y << std::endl;
                break;
        }
    }
}

void IMovable::SetRays()
{
    m_Rays->clear();

    int actualIntervalsNumber = static_cast<int>(std::ceil(Service::ConvertFromOxygineRaw(GetWidth()) / Service::Constants::RAYCAST_INTERVAL));
    long long actualIntervalLength = std::round(Service::ConvertFromOxygineRaw(GetWidth()) / actualIntervalsNumber);
    actualIntervalsNumber--;
    for (int i = 0; i < actualIntervalsNumber + 2; ++i)
    {
        // Bottom
        m_Rays->emplace_back(Collision::Ray(Service::Vector2L(GetX() + i * actualIntervalLength, GetY() + Service::ConvertFromOxygineRaw(GetHeight())),
                                          Service::Vector2L(GetX() + i * actualIntervalLength, GetY() + Service::ConvertFromOxygineRaw(GetHeight())),
                                          Collision::RayDirection::Down));
        // Top
        m_Rays->emplace_back(Collision::Ray(Service::Vector2L(GetX() + i * actualIntervalLength, GetY()),
                                          Service::Vector2L(GetX() + i * actualIntervalLength, GetY()),
                                          Collision::RayDirection::Up));
    }

    actualIntervalsNumber = static_cast<int>(std::ceil(Service::ConvertFromOxygineRaw(GetHeight()) / Service::Constants::RAYCAST_INTERVAL));
    actualIntervalLength = std::round(Service::ConvertFromOxygineRaw(GetHeight()) / actualIntervalsNumber);
    actualIntervalsNumber--;
    for (int i = 0; i < actualIntervalsNumber + 2; ++i)
    {
        // Right
        m_Rays->emplace_back(Collision::Ray(Service::Vector2L(GetX() + Service::ConvertFromOxygineRaw(GetWidth()), GetY() + i * actualIntervalLength),
                                          Service::Vector2L(GetX() + Service::ConvertFromOxygineRaw(GetWidth()), GetY() + i * actualIntervalLength),
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
    m_Rays->emplace_back(Collision::Ray(Service::Vector2L(GetX() + Service::ConvertFromOxygineRaw(GetWidth()), GetY()),
                                       Service::Vector2L(GetX() + Service::ConvertFromOxygineRaw(GetWidth()), GetY()),
                                       Collision::RayDirection::UpRight));
    // DownLeft diagonal.
    m_Rays->emplace_back(Collision::Ray(Service::Vector2L(GetX(), GetY() + Service::ConvertFromOxygineRaw(GetHeight())),
                                       Service::Vector2L(GetX(), GetY() + Service::ConvertFromOxygineRaw(GetHeight())),
                                       Collision::RayDirection::DownLeft));
    // DownRight diagonal.
    m_Rays->emplace_back(Collision::Ray(Service::Vector2L(GetX() + Service::ConvertFromOxygineRaw(GetWidth()), GetY() + Service::ConvertFromOxygineRaw(GetHeight())),
                                       Service::Vector2L(GetX() + Service::ConvertFromOxygineRaw(GetWidth()), GetY() + Service::ConvertFromOxygineRaw(GetHeight())),
                                       Collision::RayDirection::DownRight));
}

void IMovable::CheckCollisions()
{
    if (!m_CollisionManager)
    {
        return;
    }

    m_CollisionManager->CheckCollisions(this->GetId());
}
