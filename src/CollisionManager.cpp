#include <limits>

#include "CollisionManager.hpp"

void CollisionManager::AddBodies(Player* aPlayer, Static* aStatic, Static* aStatic2)
{
    m_Player = aPlayer;
    m_Static = aStatic;
    m_Static2 = aStatic2;
}

void CollisionManager::AddStatic(Static* aStatic2)
{
    m_Statics.push_back(aStatic2);
}

void CollisionManager::CheckCollisions()
{
    for (const auto& body : m_Bodies)
    {
        // Not movable.
        if (!body.second)
        {
            continue;
        }

        if (!dynamic_cast<Player*>(body.first))
        {
            std::cout << "Can't cast movable body to Player!" << body.second << std::endl;
            continue;
        }


        oxygine::Vector2 intersectionPoint;
        oxygine::Vector2 newPoint = m_Player->GetDirection();
        bool isHitDown = false;
        bool isHitRight = false;
        bool isHitUp = false;
        bool isHitLeft = false;

        for (const auto& secondBody : m_Bodies)
        {
            // Same body
            if (body.first->GetId() == secondBody.first->GetId())
            {
                continue;
            }

            if (!dynamic_cast<Static*>(secondBody.first))
            {
                std::cout << "Can't cast second body to Static!" << body.second << std::endl;
                continue;
            }

//            std::cout << "Checking body " << body.first->GetId() << " with " << secondBody.first->GetId() << std::endl;
        }

//        if (dynamic_cast<Player*>(body.first))
//        {
//            std::cout << "Can:" << body.second << std::endl;
//        }
//        else
//        {
//            std::cout << "Can't:" << body.second << std::endl;
//        }
    }


    for (auto& st : m_Statics)
    {
        for(auto& ray : m_Player->GetRays())
        {
            intersectionPoint.setZero();
            if (Intersection(
                    oxygine::Vector2(st->getX(), st->getY() + st->getHeight()),
                    oxygine::Vector2(st->getX() + st->getWidth(), st->getY()),
                    ray.Original,
                    ray.Destination,
                    intersectionPoint))
            {
                float newPos = 0;
                switch (ray.Direction)
                {
                case Collision::RayDirection::Down:
                    newPos = intersectionPoint.y - (m_Player->GetY() + m_Player->GetHeight());
                    newPoint.y = newPos > 0.01 ? newPos : 0;
                    isHitDown = true;
                    break;

                case Collision::RayDirection::Up:
                    newPos = intersectionPoint.y - m_Player->GetY();
                    newPoint.y = newPos > 0.01 ? newPos : 0;
                    isHitUp = true;
                    break;

                case Collision::RayDirection::Right:
                    newPos = intersectionPoint.x - (m_Player->GetX() + m_Player->GetWidth());
                    newPoint.x = newPos > 0.01 ? newPos : 0;
                    isHitRight = true;
                    break;

                case Collision::RayDirection::Left:
                    newPos = intersectionPoint.x - m_Player->GetX();
                    newPoint.x = newPos > 0.01 ? newPos : 0;
                    isHitLeft = true;
                    break;
                }
            }
        }
    }

    m_Player->SetDirection(newPoint);

    if (isHitDown)
    {
        m_Player->SetCollisionNormal(oxygine::Vector2(0, -1));
    }
    else if (m_Player->GetCollisionNormal().y == -1)
    {
        m_Player->SetCollisionNormal(oxygine::Vector2(0, 1));
    }

    if (isHitUp)
    {
        m_Player->SetCollisionNormal(oxygine::Vector2(0, 1));
    }
    else if (m_Player->GetCollisionNormal().y == 1)
    {
        m_Player->SetCollisionNormal(oxygine::Vector2(0, -1));
    }

    if (isHitRight)
    {
        m_Player->SetCollisionNormal(oxygine::Vector2(1, 0));
    }
    else if (m_Player->GetCollisionNormal().x == 1)
    {
        m_Player->SetCollisionNormal(oxygine::Vector2(-1, 0));
    }

    if (isHitLeft)
    {
        m_Player->SetCollisionNormal(oxygine::Vector2(-1, 0));
    }
    else if (m_Player->GetCollisionNormal().x == -1)
    {
        m_Player->SetCollisionNormal(oxygine::Vector2(1, 0));
    }
}

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
