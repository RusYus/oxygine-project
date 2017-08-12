#include "IMovable.hpp"

void IMovable::SetDirection(const oxygine::Vector2& aNewDirection)
{
    m_Direction = aNewDirection;
}

oxygine::Vector2 IMovable::GetRayOriginal() const
{
    return m_Rays.back().Original;
}

oxygine::Vector2 IMovable::GetRayDestination() const
{
    return m_Rays.back().Destination;
}
std::vector<Collision::Ray>& IMovable::GetRays()
{
    return m_Rays;
}

oxygine::Vector2 IMovable::GetDirection() const
{
    return m_Direction;
}

void IMovable::SetCollisionNormal(const oxygine::Vector2 aNormal)
{
    m_CollisionNormal += aNormal;
}

void IMovable::ResetCollisionNormal(const Collision::CollisionInfo& a_Sides)
{
    m_CollisionNormal.SetZero();

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

    UpdateRays(true);

    m_View->setPosition(newPos);
}

void IMovable::UpdateRays(bool aOriginal)
{
    for(auto& ray : m_Rays)
    {
        if (aOriginal)
        {
            ray.Original += m_Direction;
        }

        switch (ray.Direction)
        {
            case Collision::RayDirection::Up:
                if (m_Direction.y < 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x, ray.Original.y + m_Direction.y);
                }
                else
                {
                    ray.Destination = ray.Original;
                }
                break;
            case Collision::RayDirection::Down:
                if (m_Direction.y > 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x, ray.Original.y + m_Direction.y);
                }
                else
                {
                    ray.Destination = ray.Original;
                }
                break;
            case Collision::RayDirection::Right:
                if (m_Direction.x > 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x + m_Direction.x, ray.Original.y);
                }
                else
                {
                    ray.Destination = ray.Original;
                }
                break;

            case Collision::RayDirection::Left:
                if (m_Direction.x < 0)
                {
                    ray.Destination = oxygine::Vector2(ray.Original.x + m_Direction.x, ray.Original.y);
                }
                else
                {
                    ray.Destination = ray.Original;
                }
                break;
        }
    }
}

void IMovable::SetRays()
{
    m_Rays.clear();

    int actualIntervalsNumber = static_cast<int>(std::ceil(GetWidth() / Service::Constants::RAYCAST_INTERVAL));
    float actualIntervalLength = GetWidth() / actualIntervalsNumber;
    actualIntervalsNumber--;
    for (int i = 0; i < actualIntervalsNumber + 2; ++i)
    {
        // Bottom
        m_Rays.emplace_back(Collision::Ray(oxygine::Vector2(GetX() + i * actualIntervalLength, GetY() + GetHeight()),
                                          oxygine::Vector2(GetX() + i * actualIntervalLength, GetY() + GetHeight()),
                                          Collision::RayDirection::Down));
        // Top
        m_Rays.emplace_back(Collision::Ray(oxygine::Vector2(GetX() + i * actualIntervalLength, GetY()),
                                          oxygine::Vector2(GetX() + i * actualIntervalLength, GetY()),
                                          Collision::RayDirection::Up));
    }

    actualIntervalsNumber = static_cast<int>(std::ceil(GetHeight() / Service::Constants::RAYCAST_INTERVAL));
    actualIntervalLength = GetHeight() / actualIntervalsNumber;
    actualIntervalsNumber--;
    for (int i = 0; i < actualIntervalsNumber + 2; ++i)
    {
        // Right
        m_Rays.emplace_back(Collision::Ray(oxygine::Vector2(GetX() + GetWidth(), GetY() + i * actualIntervalLength),
                                          oxygine::Vector2(GetX() + GetWidth(), GetY() + i * actualIntervalLength),
                                          Collision::RayDirection::Right));
        // Left
        m_Rays.emplace_back(Collision::Ray(oxygine::Vector2(GetX(), GetY() + i * actualIntervalLength),
                                          oxygine::Vector2(GetX(), GetY() + i * actualIntervalLength),
                                          Collision::RayDirection::Left));
    }
}
