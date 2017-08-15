#pragma once

#include <vector>
#include <iostream>

#include "DemoLevel.hpp"
#include "Player.hpp"

struct CollisionBody
{
    Basis::BasisObject* m_Object = nullptr;
    bool m_IsMovable = false;
    std::vector<Service::IdGenerator::TId> m_CheckedObjects;
};

struct CollisionRectangle
{
    int X = -1;
    int Y = -1;
    int Width = -1;
    int Height = -1;
};

class CollisionManager
{
public:
    template<typename BodyType>
    void AddBody(BodyType* aBody)
    {
        CollisionBody body;
        body.m_Object = static_cast<Basis::BasisObject*>(aBody);
        // Check whether it MovableObject or not (derived from BasisObject?)
        if (std::is_base_of<IMovable, BodyType>::value)
        {
            std::cout << "Base or same type " << std::endl;
            body.m_IsMovable = true;
        }
        else
        {
            std::cout << "Not the same type " << std::endl;
            body.m_IsMovable = false;
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

        for(auto& ray : a_First->GetRays())
        {
            a_IntersectionPoint.setZero();
            if (Intersection(
                    oxygine::Vector2(m_Rectangle.X, m_Rectangle.Y + m_Rectangle.Height),
                    oxygine::Vector2(m_Rectangle.X + m_Rectangle.Width, m_Rectangle.Y),
                    ray.Original,
                    ray.Destination,
                    a_IntersectionPoint))
            {
                float newPos = 0;
                switch (ray.Direction)
                {
                case Collision::RayDirection::Down:
                    newPos = a_IntersectionPoint.y - (a_First->GetY() + a_First->GetHeight());
                    a_NewPoint.y = newPos > 0.01 ? newPos : 0;
                    a_Sides.Down = true;
                    break;

                case Collision::RayDirection::Up:
                    newPos = a_IntersectionPoint.y - a_First->GetY();
                    a_NewPoint.y = newPos > 0.01 ? newPos : 0;
                    a_Sides.Up = true;
                    break;

                case Collision::RayDirection::Right:
                    newPos = a_IntersectionPoint.x - (a_First->GetX() + a_First->GetWidth());
                    a_NewPoint.x = newPos > 0.01 ? newPos : 0;
                    a_Sides.Right = true;
                    break;

                case Collision::RayDirection::Left:
                    newPos = a_IntersectionPoint.x - a_First->GetX();
                    a_NewPoint.x = newPos > 0.01 ? newPos : 0;
                    a_Sides.Left = true;
                    break;

                // TODO : Refactor!
                case Collision::RayDirection::UpRight:
                    // If exactly on corner, considering it under (have to choose between under and right).
                    if (a_IntersectionPoint.x == m_Rectangle.X)
                    {
                        // From Right
                        float x = a_IntersectionPoint.x - (a_First->GetX() + a_First->GetWidth());
                        a_NewPoint.x = x > 0.01 ? x : 0;
                        a_Sides.Right = true;
                    }
                    else
                    {
                        // From up
                        float y = a_IntersectionPoint.y - a_First->GetY();
                        a_NewPoint.y = y > 0.01 ? y : 0;
                        a_Sides.Up = true;
                    }
                    break;

                case Collision::RayDirection::UpLeft:
                    // If exactly on corner, considering it under (have to choose between under and left).
                    if (a_IntersectionPoint.x == m_Rectangle.X + m_Rectangle.Width)
                    {
                        // From Left
                        float x = a_IntersectionPoint.x - a_First->GetX();
                        a_NewPoint.x = x > 0.01 ? x : 0;
                        a_Sides.Left = true;
                    }
                    else
                    {
                        // From up
                        float y = a_IntersectionPoint.y - a_First->GetY();
                        a_NewPoint.y = y > 0.01 ? y : 0;
                        a_Sides.Up = true;
                    }
                    break;

                case Collision::RayDirection::DownRight:
                    // If exactly on corner, considering it on top (have to choose between on top and right).
                    if (a_IntersectionPoint.x == m_Rectangle.X)
                    {
                        // From Right
                        float x = a_IntersectionPoint.x - (a_First->GetX() + a_First->GetWidth());
                        a_NewPoint.x = x > 0.01 ? x : 0;
                        a_Sides.Right = true;
                    }
                    else
                    {
                        // From down
                        float y = a_IntersectionPoint.y - (a_First->GetY() + a_First->GetHeight());
                        a_NewPoint.y = y > 0.01 ? y : 0;
                        a_Sides.Down = true;
                    }
                    break;

                case Collision::RayDirection::DownLeft:
                    // If exactly on corner, considering it on top (have to choose between on top and left).
                    if (a_IntersectionPoint.x == m_Rectangle.X + m_Rectangle.Width)
                    {
                        // From Left
                        float x = a_IntersectionPoint.x - a_First->GetX();
                        a_NewPoint.x = x > 0.01 ? x : 0;
                        a_Sides.Left = true;
                    }
                    else
                    {
                        // From down
                        float y = a_IntersectionPoint.y - (a_First->GetY() + a_First->GetHeight());
                        a_NewPoint.y = y > 0.01 ? y : 0;
                        a_Sides.Down = true;
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
    std::vector<CollisionBody> m_Bodies;
    CollisionRectangle m_Rectangle;
};
