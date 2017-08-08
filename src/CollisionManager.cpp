#include <limits>

#include "CollisionManager.hpp"

void CollisionManager::CheckCollisions()
{
    // TODO : Optimizations checks for collisions (quad tree or four-areas on screen?).
    for (const auto& body : m_Bodies)
    {
        // Not movable.
        if (!body.second)
        {
            continue;
        }

        Player* player = dynamic_cast<Player*>(body.first);
        if (!player)
        {
            std::cout << "Can't cast movable body to Player!" << body.second << std::endl;
            continue;
        }


        oxygine::Vector2 intersectionPoint;
        oxygine::Vector2 newPoint = player->GetDirection();
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

            Static* ground = dynamic_cast<Static*>(secondBody.first);
            if (!ground)
            {
                std::cout << "Can't cast second body to Static!" << body.second << std::endl;
                continue;
            }

            for(auto& ray : player->GetRays())
            {
                intersectionPoint.setZero();
                if (Intersection(
                        oxygine::Vector2(ground->getX(), ground->getY() + ground->getHeight()),
                        oxygine::Vector2(ground->getX() + ground->getWidth(), ground->getY()),
                        ray.Original,
                        ray.Destination,
                        intersectionPoint))
                {
                    float newPos = 0;
                    switch (ray.Direction)
                    {
                    case Collision::RayDirection::Down:
                        newPos = intersectionPoint.y - (player->GetY() + player->GetHeight());
                        newPoint.y = newPos > 0.01 ? newPos : 0;
                        isHitDown = true;
                        break;

                    case Collision::RayDirection::Up:
                        newPos = intersectionPoint.y - player->GetY();
                        newPoint.y = newPos > 0.01 ? newPos : 0;
                        isHitUp = true;
                        break;

                    case Collision::RayDirection::Right:
                        newPos = intersectionPoint.x - (player->GetX() + player->GetWidth());
                        newPoint.x = newPos > 0.01 ? newPos : 0;
                        isHitRight = true;
                        break;

                    case Collision::RayDirection::Left:
                        newPos = intersectionPoint.x - player->GetX();
                        newPoint.x = newPos > 0.01 ? newPos : 0;
                        isHitLeft = true;
                        break;
                    }
                }
            }

//            std::cout << "Checking body " << body.first->GetId() << " with " << secondBody.first->GetId() << std::endl;
        }

        player->SetDirection(newPoint);

        if (isHitDown)
        {
            player->SetCollisionNormal(oxygine::Vector2(0, -1));
        }
        else if (player->GetCollisionNormal().y == -1)
        {
            player->SetCollisionNormal(oxygine::Vector2(0, 1));
        }

        if (isHitUp)
        {
            player->SetCollisionNormal(oxygine::Vector2(0, 1));
        }
        else if (player->GetCollisionNormal().y == 1)
        {
            player->SetCollisionNormal(oxygine::Vector2(0, -1));
        }

        if (isHitRight)
        {
            player->SetCollisionNormal(oxygine::Vector2(1, 0));
        }
        else if (player->GetCollisionNormal().x == 1)
        {
            player->SetCollisionNormal(oxygine::Vector2(-1, 0));
        }

        if (isHitLeft)
        {
            player->SetCollisionNormal(oxygine::Vector2(-1, 0));
        }
        else if (player->GetCollisionNormal().x == -1)
        {
            player->SetCollisionNormal(oxygine::Vector2(1, 0));
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
