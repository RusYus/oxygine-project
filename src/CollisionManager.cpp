#include <iostream>
#include <limits>

#include "CollisionManager.hpp"

void CollisionManager::AddBody(void* aBody)
{
    m_Bodies.push_back(aBody);
}

void CollisionManager::AddBodies(Player* aPlayer, Static* aStatic, Static* aStatic2)
{
    m_Player = aPlayer;
    m_Static = aStatic;
    m_Static2 = aStatic2;
}

void CollisionManager::CheckCollisions()
{
    oxygine::Vector2 intersectionPoint;

    for(auto& ray : m_Player->GetRays())
    {
        ray.IsHitInLastStep = ray.IsHitInCurrentStep;
        if (ray.Original == ray.Destination)
        {
            ray.IsHitInCurrentStep = false;
            if (ray.IsHitInLastStep && !ray.IsHitInCurrentStep
                    && m_Player->GetCollisionNormal().y == -1)
            {
                m_Player->SetCollisionNormal(oxygine::Vector2(0, 1));
                std::cout << "Unsetting normal)" << ray.Original.x << ":" << ray.Original.y << std::endl;
            }
            continue;
        }

        intersectionPoint.setZero();
        if (Intersection(
            oxygine::Vector2(m_Static->getX(), m_Static->getY() + m_Static->getHeight()),
            oxygine::Vector2(m_Static->getX() + m_Static->getWidth(), m_Static->getY()),
            ray.Original,
            ray.Destination,
            intersectionPoint))
        {
//            std::cout << "Collision took place! " << intersectionPoint.x << ":" << intersectionPoint.y << std::endl;
            float newPosY = intersectionPoint.y - (m_Player->GetY() + m_Player->GetHeight());
            if (newPosY < 0.1 && newPosY > 0)
            std::cout << "NewPosY:" << newPosY << std::endl;

            ray.IsHitInCurrentStep = true;

            m_Player->SetY(newPosY > 0.01 ? newPosY : 0);
            m_Player->SetCollisionNormal(oxygine::Vector2(0, -1));
        }
        else
        {
            std::cout << "else" << std::endl;
            ray.IsHitInCurrentStep = false;
            // TODO : "proper" unsetting collision normal.
            if (ray.IsHitInLastStep && !ray.IsHitInCurrentStep
                    && m_Player->GetCollisionNormal().y == -1)
            {
                m_Player->SetCollisionNormal(oxygine::Vector2(0, 1));
                std::cout << "Unsetting normal)" << ray.Original.x << ":" << ray.Original.y << std::endl;
            }
        }

        // These flags: IsHitInCurrentStep not gonna work in current realisation
        // beacause next bodyies (now only m_static2) overwrites it, so ray never hit anything.

//        intersectionPoint.setZero();

//        if (Intersection(
//            oxygine::Vector2(m_Static2->getX(), m_Static2->getY() + m_Static2->getHeight()),
//            oxygine::Vector2(m_Static2->getX() + m_Static2->getWidth(), m_Static2->getY()),
//            ray.Original,
//            ray.Destination,
//            intersectionPoint))
//        {
////            std::cout << "Collision took place! " << intersectionPoint.x << ":" << intersectionPoint.y << std::endl;
//            float newPosX = intersectionPoint.x - (m_Player->GetX() + m_Player->GetWidth());
////            std::cout << "NewPosY:" << newPosY << std::endl;

//            ray.IsHitInCurrentStep = true;

//            m_Player->SetX(newPosX);
//            m_Player->SetCollisionNormal(oxygine::Vector2(1, 0));
//        }
//        else
//        {
//            ray.IsHitInCurrentStep = false;
//            // TODO : "proper" unsetting collision normal.
//            if (ray.IsHitInLastStep && !ray.IsHitInCurrentStep
//                    && m_Player->GetCollisionNormal().x == 1)
//            {
//                m_Player->SetCollisionNormal(oxygine::Vector2(-1, 0));
//                std::cout << "Unsetting normal)" << ray.Original.x << ":" << ray.Original.y << std::endl;
//            }
//        }
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

    if (f_low >= f_high)
        return false;

    oxygine::Vector2 b = aEndRay - aStartRay;

    outIntersection = aStartRay + b * f_low;

    return true;

}
