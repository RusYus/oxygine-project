#pragma once

#include <vector>
#include <iostream>

#include "ICollisionManager.hpp"
#include "DemoLevel.hpp"
#include "Player.hpp"

// 1 - Don't check collisions for platform
// 2 - Check intersection (handle carrier?) to add passengers
// 3 - Every carrier has own passengers
// 4 - Move passengers before Update (adding direction)
// 5 - Move everything as usual

class CollisionManager : public virtual ICollisionManager
{
    using TBody = std::pair<Basis::BasisObject*, bool /*isMovable*/>;
    using TKey = Basis::BasisObject::TId;
    using TValue = TBody;
    struct CollisionRectangle
    {
        Service::TCoordinate X = std::numeric_limits<int>::quiet_NaN();
        Service::TCoordinate Y = std::numeric_limits<int>::quiet_NaN();
        int Width = -1;
        int Height = -1;
        int WidthWithDirection = -1;
        int HeightWithDirection = -1;
    };
public:
    CollisionManager() = default;
    ~CollisionManager() = default;

    void AddBody(Basis::BasisObject* a_Body) override
    {
        TBody body = std::make_pair(a_Body, false);
        // TODO : refactor condition.
        if (std::is_base_of<IMovable, TBody>::value /*&& !dynamic_cast<Platform*>(aBody)*/)
        {
            body.second = true;
        }

        m_Bodies.emplace(body.first->GetId(), std::move(body));
    }

    void CheckCollisions(Basis::BasisObject::TId) override;
private:
    template<typename FirstBody>
    void HandleIntersection(
        FirstBody* a_First,
        Collision::CollisionInfo& a_Sides,
        Service::Vector2L& a_IntersectionPoint,
        Service::Vector2L& a_NewPoint)
    {
        if (m_Rectangle.Width <= 0 || m_Rectangle.Height <= 0)
        {
            std::cout << "Negative size" << std::endl;
            return;
        }

        auto handleRight = [&a_First, &a_Sides, &a_IntersectionPoint, &a_NewPoint] ()
        {
            if (a_IntersectionPoint.x == std::numeric_limits<float>::quiet_NaN()
                || a_IntersectionPoint.y == std::numeric_limits<float>::quiet_NaN())
            {
                return;
            }

            float newPos = a_IntersectionPoint.x - (a_First->GetX() + a_First->GetWidth());
            a_NewPoint.x = newPos > 1 ? newPos : 0;
            a_Sides.Right = true;
        };

        auto handleLeft = [&a_First, &a_Sides, &a_IntersectionPoint, &a_NewPoint] ()
        {
            if (a_IntersectionPoint.x == std::numeric_limits<float>::quiet_NaN()
                || a_IntersectionPoint.y == std::numeric_limits<float>::quiet_NaN())
            {
                return;
            }

            float newPos = a_IntersectionPoint.x - a_First->GetX();
            a_NewPoint.x = newPos > 1 ? newPos : 0;
            a_Sides.Left = true;
        };

        auto handleUp = [&a_First, &a_Sides, &a_IntersectionPoint, &a_NewPoint] ()
        {
            if (a_IntersectionPoint.x == std::numeric_limits<float>::quiet_NaN()
                || a_IntersectionPoint.y == std::numeric_limits<float>::quiet_NaN())
            {
                return;
            }

            float newPos = a_IntersectionPoint.y - a_First->GetY();
            a_NewPoint.y = newPos > 1 ? newPos : 0;
            a_Sides.Up = true;
        };

        auto handleDown = [&a_First, &a_Sides, &a_IntersectionPoint, &a_NewPoint] ()
        {
            if (a_IntersectionPoint.x == std::numeric_limits<float>::quiet_NaN()
                || a_IntersectionPoint.y == std::numeric_limits<float>::quiet_NaN())
            {
                return;
            }

            float newPos = a_IntersectionPoint.y - (a_First->GetY() + a_First->GetHeight());
            a_NewPoint.y = newPos > 1 ? newPos : 0;
            a_Sides.Down = true;
        };

        for(const auto& ray : *(a_First->GetRays()))
        {
            a_IntersectionPoint.setZero();
            // Don't need to check in that direction, since I assume, that if coords are the same
            // means no moving there.
            if (ray.Original == ray.Destination)
            {
                continue;
            }
            if (Intersection(
                    Service::Vector2L(m_Rectangle.X, m_Rectangle.Y + m_Rectangle.Height),
                    Service::Vector2L(m_Rectangle.X + m_Rectangle.Width, m_Rectangle.Y),
                    ray.Original,
                    ray.Destination,
                    a_IntersectionPoint))
            {
                switch (ray.Direction)
                {
                case Collision::RayDirection::Down:
                    handleDown();
                    break;

                case Collision::RayDirection::Up:
                    handleUp();
                    break;

                case Collision::RayDirection::Right:
                    handleRight();
                    break;

                case Collision::RayDirection::Left:
                    handleLeft();
                    break;

                case Collision::RayDirection::UpRight:
                    // If exactly on corner, considering it under (have to choose between under and right).
                    if (a_IntersectionPoint.x == m_Rectangle.X)
                    {
                        handleRight();
                    }
                    else
                    {
                        handleUp();
                    }
                    break;

                case Collision::RayDirection::UpLeft:
                    // If exactly on corner, considering it under (have to choose between under and left).
                    if (a_IntersectionPoint.x == m_Rectangle.X + m_Rectangle.Width)
                    {
                        handleLeft();
                    }
                    else
                    {
                        handleUp();
                    }
                    break;

                case Collision::RayDirection::DownRight:
                    // If exactly on corner, considering it on top (have to choose between on top and right).
                    if (a_IntersectionPoint.x == m_Rectangle.X)
                    {
                        handleRight();
                    }
                    else
                    {
                        handleDown();
                    }
                    break;

                case Collision::RayDirection::DownLeft:
                    // If exactly on corner, considering it on top (have to choose between on top and left).
                    if (a_IntersectionPoint.x == m_Rectangle.X + m_Rectangle.Width)
                    {
                        handleLeft();
                    }
                    else
                    {
                        handleDown();
                    }
                    break;
                }
            }
        }
    }

    template<typename FirstBody>
    bool HandleCarrierIntersection(FirstBody* a_First, Service::Vector2L& a_NewPoint)
    {
        static_assert(std::is_base_of<ICarrier, FirstBody>::value, "Should be used with ICarrier or it's child!");

        if (m_Rectangle.Width <= 0 || m_Rectangle.Height <= 0
            || m_Rectangle.WidthWithDirection <= 0 || m_Rectangle.HeightWithDirection <= 0)
        {
            std::cout << "Negative size" << std::endl;
            return false;
        }

        for(const auto& ray : *(a_First->GetCarrierRays()))
        {
            // Don't need to check in that direction, since I assume, that if coords are the same
            // means no moving there.
            if (ray.Original == ray.Destination)
            {
                continue;
            }

            Service::Vector2L intersectionPoint;

            if (Intersection(
                    Service::Vector2L(m_Rectangle.X, m_Rectangle.Y + m_Rectangle.HeightWithDirection),
                    Service::Vector2L(m_Rectangle.X + m_Rectangle.WidthWithDirection, m_Rectangle.Y),
                    ray.Original,
                    ray.Destination,
                    intersectionPoint))
            {
                if (intersectionPoint.x == std::numeric_limits<float>::quiet_NaN()
                    || intersectionPoint.y == std::numeric_limits<float>::quiet_NaN())
                {
                    return false;
                }

                a_NewPoint.y = intersectionPoint.y -  m_Rectangle.Y - m_Rectangle.Height;
                return true;
            }
        }

        return false;
    }

    bool Intersection(
        const Service::Vector2L& /*bottomLeftAABB*/, const Service::Vector2L& /*topRightAABB*/,
        const Service::Vector2L& /*startRay*/, const Service::Vector2L& /*endRay*/,
        Service::Vector2L& /*intersection*/);

private:
    std::unordered_map<TKey, TValue> m_Bodies;
    CollisionRectangle m_Rectangle;
};
