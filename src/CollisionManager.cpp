#include <limits>

#include "CollisionManager.hpp"

void CollisionManager::CheckCollisions()
{
    Collision::CollisionInfo collisionSides;

    m_Rectangle.X = 0;
    m_Rectangle.Y = 0;
    m_Rectangle.Width = -1;
    m_Rectangle.Height = -1;

    // TODO : Optimizations checks for collisions (quad tree or four-areas on screen?).
    for (auto& body : m_Bodies)
    {
        // Not movable.
        if (!body.second)
        {
            continue;
        }

        collisionSides.Reset();

        IMovable* firstBody = dynamic_cast<IMovable*>(body.first);
        if (!firstBody)
        {
            std::cout << "Can't cast to movable body!" << std::endl;
            continue;
        }

        oxygine::Vector2 intersectionPoint;
        oxygine::Vector2 newPoint = firstBody->GetDirection();

        const auto bodyId = firstBody->GetId();

        for (auto& secondBody : m_Bodies)
        {
            // Same body
            if (bodyId == secondBody.first->GetId())
            {
                continue;
            }

            if (dynamic_cast<Static*>(secondBody.first))
            {
                Static* ground = dynamic_cast<Static*>(secondBody.first);
                m_Rectangle.X = ground->GetX();
                m_Rectangle.Y = ground->GetY();
                m_Rectangle.Width = ground->GetWidth();
                m_Rectangle.Height = ground->GetHeight();
            }
            else if (dynamic_cast<IMovable*>(secondBody.first))
            {
                IMovable* movableBody = dynamic_cast<IMovable*>(secondBody.first);
                oxygine::Vector2 minCoords{movableBody->GetX(), movableBody->GetY()};
                oxygine::Vector2 maxCoords{movableBody->GetX(), movableBody->GetY()};

                // Calculating boundaries of the object out of it's rays (including destination).
                // It's gonna be aabb for first body to check collision with.
                auto checkMin = [&minCoords] (const auto& a_Ray)
                {
                    if (a_Ray.Original.x < minCoords.x)
                    {
                        minCoords.x = a_Ray.Original.x;
                    }

                    if (a_Ray.Original.y < minCoords.y)
                    {
                        minCoords.y = a_Ray.Original.y;
                    }

                    if (a_Ray.Destination.x < minCoords.x)
                    {
                        minCoords.x = a_Ray.Destination.x;
                    }

                    if (a_Ray.Destination.y < minCoords.y)
                    {
                        minCoords.y = a_Ray.Destination.y;
                    }
                };

                auto checkMax = [&maxCoords] (const auto& a_Ray)
                {
                    if (a_Ray.Original.x > maxCoords.x)
                    {
                        maxCoords.x = a_Ray.Original.x;
                    }

                    if (a_Ray.Original.y > maxCoords.y)
                    {
                        maxCoords.y = a_Ray.Original.y;
                    }

                    if (a_Ray.Destination.x > maxCoords.x)
                    {
                        maxCoords.x = a_Ray.Destination.x;
                    }

                    if (a_Ray.Destination.y > maxCoords.y)
                    {
                        maxCoords.y = a_Ray.Destination.y;
                    }
                };

                std::for_each(movableBody->GetRays()->cbegin(), movableBody->GetRays()->cend(), checkMin);
                std::for_each(movableBody->GetRays()->cbegin(), movableBody->GetRays()->cend(), checkMax);

                m_Rectangle.X = minCoords.x;
                m_Rectangle.Y = minCoords.y;
                m_Rectangle.Width = maxCoords.x - minCoords.x;
                m_Rectangle.Height = maxCoords.y - minCoords.y;
            }
            else
            {
                std::cout << "Can't cast second body!" << std::endl;
                continue;
            }

            if (dynamic_cast<Platform*>(firstBody))
            {
                ICarrier* carrier = dynamic_cast<Platform*>(firstBody);
                if (HandleCarrierIntersection(carrier))
                {
                    Player* player = dynamic_cast<Player*>(secondBody.first);
                    oxygine::Vector2 newDirectionForPlayer;
//                    newDirectionForPlayer.x = player->GetDirection().x;
                    newDirectionForPlayer.x = carrier->GetDirection().x;
                    newDirectionForPlayer.y = carrier->GetY() - player->GetY() - player->GetHeight();
                    player->SetDirectionFinalForStep(newDirectionForPlayer);
                    continue;
                }
            }

            HandleIntersection(firstBody, collisionSides, intersectionPoint, newPoint);
        }

        firstBody->SetDirection(newPoint);
        firstBody->ResetCollisionNormal(collisionSides);
    }
}

// TODO : Use better names, more comments.
bool CollisionManager::Intersection(
        const oxygine::Vector2& aBottomLeftAABB, const oxygine::Vector2& aTopRightAABB,
        const oxygine::Vector2& aStartRay, const oxygine::Vector2& aEndRay,
        oxygine::Vector2& outIntersection)
{
    float f_low = 0;
    float f_high = 1;

    float f_dim_low = 0;
    float f_dim_high = 1;

    float q1 = aBottomLeftAABB.x - aStartRay.x;
    float q2  =aTopRightAABB.x - aStartRay.x;
    float q = aEndRay.x - aStartRay.x;

    if (q == 0)
    {
        f_dim_low = q1 > 0 ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity();
    }
    else
    {
        f_dim_low = q1 / q;
    }

    if (q == 0)
    {
        f_dim_high = q2 > 0 ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity();;
    }
    else
    {
        f_dim_high = q2 / q;
    }

    if (f_dim_high < f_dim_low)
    {
        std::swap(f_dim_high, f_dim_low);
    }

    if (f_dim_high < f_low)
        return false;

    if (f_dim_low > f_high)
        return false;

    f_low = std::max(f_dim_low, f_low);
    f_high = std::min(f_dim_high, f_high);

    if (f_low > f_high)
        return false;


    // ------ y -------

    q = aEndRay.y - aStartRay.y;
    q1 = aBottomLeftAABB.y - aStartRay.y;
    q2 = aTopRightAABB.y - aStartRay.y;

    if (q == 0)
    {
        f_dim_low = q1 > 0 ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity();
    }
    else
    {
        f_dim_low = (aBottomLeftAABB.y - aStartRay.y) / q;
    }

    if (q == 0)
    {
        f_dim_high = q2 > 0 ? std::numeric_limits<float>::infinity() : -std::numeric_limits<float>::infinity();
    }
    else
    {
        f_dim_high = q2 / q;
    }

    if (f_dim_high < f_dim_low)
    {
        std::swap(f_dim_high, f_dim_low);
    }

    if (f_dim_high < f_low)
        return false;

    if (f_dim_low > f_high)
        return false;

    f_low = std::max(f_dim_low, f_low);
    f_high = std::min(f_dim_high, f_high);

    if (f_low > f_high)
        return false;

    oxygine::Vector2 b = aEndRay - aStartRay;


    outIntersection = aStartRay + b * f_low;
    // Check if outIntersection == aStartRay. In this case aEndRay must be outside of aabb (not even on the border)
    // For example, when jumping, end ray points outside of aabb (so there will be no collision).
    // As opposite of when standing still, where end ray points into aabb.

    if (outIntersection == aStartRay)
    {
        return
                (std::round(aBottomLeftAABB.x) < std::round(aEndRay.x) && std::round(aEndRay.x) < std::round(aTopRightAABB.x)
                 && std::round(aTopRightAABB.y) < std::round(aEndRay.y) && std::round(aEndRay.y) < std::round(aBottomLeftAABB.y));
    }

    return true;

}
