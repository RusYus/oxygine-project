#include <iostream>
#include "IMovable.hpp"

IMovable::IMovable()
    : m_CollisionNormal(0, 0)
    , m_Rays(std::make_shared<std::vector<Collision::Ray>>())
{
}

IMovable::~IMovable()
{
    m_Rays->clear();
}

void IMovable::SetDirection(const oxygine::Vector2& aNewDirection)
{
    m_Direction = aNewDirection;
}

std::shared_ptr<std::vector<Collision::Ray>> IMovable::GetRays() const
{
    return m_Rays;
}

oxygine::Vector2 IMovable::GetDirection() const
{
    return m_Direction;
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
    oxygine::Vector2 newPos = GetPosition() + m_Direction;

    for(auto& ray : *m_Rays)
    {
        ray.Original += m_Direction;
    }

    m_View->setPosition(newPos);
}

void IMovable::UpdateRays()
{
    for(auto& ray : *m_Rays)
    {
        ray.Destination = ray.Original;

        switch (ray.Direction)
        {
            case Collision::RayDirection::Up:
                if (m_Direction.y < 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x, ray.Original.y + m_Direction.y);
                }
                break;
            case Collision::RayDirection::Down:
                if (m_Direction.y > 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x, ray.Original.y + m_Direction.y);
                }
                break;
            case Collision::RayDirection::Right:
                if (m_Direction.x > 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x + m_Direction.x, ray.Original.y);
                }
                break;

            case Collision::RayDirection::Left:
                if (m_Direction.x < 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x + m_Direction.x, ray.Original.y);
                }
                break;
            case Collision::RayDirection::UpLeft:
                if (m_Direction.x < 0 && m_Direction.y < 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original + m_Direction);
                }
                break;
            case Collision::RayDirection::UpRight:
                if (m_Direction.x > 0 && m_Direction.y < 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original + m_Direction);
                }
                break;
            case Collision::RayDirection::DownLeft:
                if (m_Direction.x < 0 && m_Direction.y > 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original + m_Direction);
                }
                break;
            case Collision::RayDirection::DownRight:
                if (m_Direction.x > 0 && m_Direction.y > 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original + m_Direction);
                }
                break;
        }
    }
}

void IMovable::SetRays()
{
    m_Rays->clear();

    int actualIntervalsNumber = static_cast<int>(std::ceil(GetWidth() / Service::Constants::RAYCAST_INTERVAL));
    float actualIntervalLength = GetWidth() / actualIntervalsNumber;
    actualIntervalsNumber--;
    for (int i = 0; i < actualIntervalsNumber + 2; ++i)
    {
        // Bottom
        m_Rays->emplace_back(Collision::Ray(oxygine::Vector2(GetX() + i * actualIntervalLength, GetY() + GetHeight()),
                                          oxygine::Vector2(GetX() + i * actualIntervalLength, GetY() + GetHeight()),
                                          Collision::RayDirection::Down));
        // Top
        m_Rays->emplace_back(Collision::Ray(oxygine::Vector2(GetX() + i * actualIntervalLength, GetY()),
                                          oxygine::Vector2(GetX() + i * actualIntervalLength, GetY()),
                                          Collision::RayDirection::Up));
    }

    actualIntervalsNumber = static_cast<int>(std::ceil(GetHeight() / Service::Constants::RAYCAST_INTERVAL));
    actualIntervalLength = GetHeight() / actualIntervalsNumber;
    actualIntervalsNumber--;
    for (int i = 0; i < actualIntervalsNumber + 2; ++i)
    {
        // Right
        m_Rays->emplace_back(Collision::Ray(oxygine::Vector2(GetX() + GetWidth(), GetY() + i * actualIntervalLength),
                                          oxygine::Vector2(GetX() + GetWidth(), GetY() + i * actualIntervalLength),
                                          Collision::RayDirection::Right));
        // Left
        m_Rays->emplace_back(Collision::Ray(oxygine::Vector2(GetX(), GetY() + i * actualIntervalLength),
                                          oxygine::Vector2(GetX(), GetY() + i * actualIntervalLength),
                                          Collision::RayDirection::Left));
    }

    // UpLeft diagonal.
    m_Rays->emplace_back(Collision::Ray(oxygine::Vector2(GetX(), GetY()),
                                       oxygine::Vector2(GetX(), GetY()),
                                       Collision::RayDirection::UpLeft));
    // UpRight diagonal.
    m_Rays->emplace_back(Collision::Ray(oxygine::Vector2(GetX() + GetWidth(), GetY()),
                                       oxygine::Vector2(GetX() + GetWidth(), GetY()),
                                       Collision::RayDirection::UpRight));
    // DownLeft diagonal.
    m_Rays->emplace_back(Collision::Ray(oxygine::Vector2(GetX(), GetY() + GetHeight()),
                                       oxygine::Vector2(GetX(), GetY() + GetHeight()),
                                       Collision::RayDirection::DownLeft));
    // DownRight diagonal.
    m_Rays->emplace_back(Collision::Ray(oxygine::Vector2(GetX() + GetWidth(), GetY() + GetHeight()),
                                       oxygine::Vector2(GetX() + GetWidth(), GetY() + GetHeight()),
                                       Collision::RayDirection::DownRight));
}
