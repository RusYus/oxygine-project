#pragma once

#include <vector>
#include <iostream>

#include "DemoLevel.hpp"
#include "Player.hpp"

class CollisionManager
{
    using TBody = std::pair<Basis::BasisObject*, bool /*isMovable*/>;
    struct CollisionRectangle
    {
        int X = -1;
        int Y = -1;
        int Width = -1;
        int Height = -1;
    };
public:
    template<typename BodyType>
    void AddBody(BodyType* aBody)
    {
        TBody body = std::make_pair(static_cast<Basis::BasisObject*>(aBody), false);
        if (std::is_base_of<IMovable, BodyType>::value)
        {
            body.second = true;
        }

        m_Bodies.emplace_back(std::move(body));
    }

    void CheckCollisions();
private:
    template<typename FirstBody>
    void HandleIntersection(
        FirstBody* a_First,
        Collision::CollisionInfo& a_Sides,
        oxygine::Vector2& a_IntersectionPoint,
        oxygine::Vector2& a_NewPoint)
    {
        if (m_Rectangle.Width <= 0 || m_Rectangle.Height <= 0)
        {
            std::cout << "Negative size" << std::endl;
            return;
        }

        auto handleRight = [&a_First, &a_Sides, &a_IntersectionPoint, &a_NewPoint] ()
        {
            float newPos = a_IntersectionPoint.x - (a_First->GetX() + a_First->GetWidth());
            a_NewPoint.x = newPos > 0.01 ? newPos : 0;
            a_Sides.Right = true;
        };

        auto handleLeft = [&a_First, &a_Sides, &a_IntersectionPoint, &a_NewPoint] ()
        {
            float newPos = a_IntersectionPoint.x - a_First->GetX();
            a_NewPoint.x = newPos > 0.01 ? newPos : 0;
            a_Sides.Left = true;
        };

        auto handleUp = [&a_First, &a_Sides, &a_IntersectionPoint, &a_NewPoint] ()
        {
            float newPos = a_IntersectionPoint.y - a_First->GetY();
            a_NewPoint.y = newPos > 0.01 ? newPos : 0;
            a_Sides.Up = true;
        };

        auto handleDown = [&a_First, &a_Sides, &a_IntersectionPoint, &a_NewPoint] ()
        {
            float newPos = a_IntersectionPoint.y - (a_First->GetY() + a_First->GetHeight());
            a_NewPoint.y = newPos > 0.01 ? newPos : 0;
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
                    oxygine::Vector2(m_Rectangle.X, m_Rectangle.Y + m_Rectangle.Height),
                    oxygine::Vector2(m_Rectangle.X + m_Rectangle.Width, m_Rectangle.Y),
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

    bool Intersection(
        const oxygine::Vector2& /*bottomLeftAABB*/, const oxygine::Vector2& /*topRightAABB*/,
        const oxygine::Vector2& /*startRay*/, const oxygine::Vector2& /*endRay*/,
        oxygine::Vector2& /*intersection*/);

private:
    std::vector<TBody> m_Bodies;
    CollisionRectangle m_Rectangle;
};
