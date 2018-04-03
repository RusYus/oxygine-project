#pragma once

#include <vector>
#include <iostream>

#include "ICarrier.hpp"
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
    enum class BodyType
    {
        Platform,
        Player,
        DynamicBox,
    };

    using TBody = std::pair<Basis::BasisObject*, bool /*isMovable*/>;
    using TKey = Basis::BasisObject::TId;
    using TValue = TBody;
    struct CollisionRectangle
    {
        int Width = -1;
        int Height = -1;
        Service::Vector2L bottomLeft{std::numeric_limits<Service::TCoordinate>::quiet_NaN(), std::numeric_limits<Service::TCoordinate>::quiet_NaN()};
        Service::Vector2L topRight{std::numeric_limits<Service::TCoordinate>::quiet_NaN(), std::numeric_limits<Service::TCoordinate>::quiet_NaN()};
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

//        m_Bodies.emplace(body.first->GetId(), std::move(body));
        m_Bodies.emplace(body.first->GetId(), body);
    }

    void PrintCarrierId()
    {
        for (const auto& body : m_Bodies)
        {
            const auto bodyP = dynamic_cast<IMovable*>(body.second.first);
            if (bodyP)
            {
                std::cout << bodyP->GetId() << " : " << bodyP->CarrierInfo.Id << "; Address: " << bodyP << std::endl;
            }
        }
    }

    void CheckCollisions(Basis::BasisObject::TId) override;
private:
    void FillRectangleValues(Basis::BasisObject&);
    void UpdateRectangleWithDirection(IMovable&);

    template<typename FirstBody>
    void HandleIntersection(
        FirstBody* a_First,
        Collision::CollisionInfo& a_Sides,
        Service::Vector2L& a_IntersectionPoint,
        Service::Vector2L& a_NewPoint)
    {
        if (m_Rectangle.topRight.x <= m_Rectangle.bottomLeft.x
            || m_Rectangle.bottomLeft.y <= m_Rectangle.topRight.y)
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
            a_NewPoint.x = std::abs(newPos) > 1 ? newPos : 0;
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
            a_NewPoint.x = std::abs(newPos) > 1 ? newPos : 0;
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
            a_NewPoint.y = std::abs(newPos) > 1 ? newPos : 0;
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
            a_NewPoint.y = std::abs(newPos) > 1 ? newPos : 0;
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
                m_Rectangle.bottomLeft,
                m_Rectangle.topRight,
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
                    if (a_IntersectionPoint.x == m_Rectangle.bottomLeft.x)
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
                    if (a_IntersectionPoint.x == m_Rectangle.topRight.x)
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
                    if (a_IntersectionPoint.x == m_Rectangle.bottomLeft.x)
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
                    if (a_IntersectionPoint.x == m_Rectangle.topRight.x)
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

        if (m_Rectangle.topRight.x <= m_Rectangle.bottomLeft.x
            || m_Rectangle.bottomLeft.y <= m_Rectangle.topRight.y)
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
                m_Rectangle.bottomLeft,
                m_Rectangle.topRight,
                ray.Original,
                ray.Destination,
                intersectionPoint))
            {
                if (intersectionPoint.x == std::numeric_limits<float>::quiet_NaN()
                    || intersectionPoint.y == std::numeric_limits<float>::quiet_NaN())
                {
                    return false;
                }

                // This means that passenger coming from above.
                if (intersectionPoint.y != m_Rectangle.bottomLeft.y)
                {
                    a_NewPoint.y = intersectionPoint.y -  m_Rectangle.topRight.y - m_Rectangle.Height;
                }
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
