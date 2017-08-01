#include <iostream>
#include <limits>

#include "CollisionManager.hpp"

void CollisionManager::AddBody(void* aBody)
{
    m_Bodies.push_back(aBody);
}

void CollisionManager::AddBodies(Player* aPlayer, Static* aStatic)
{
    m_Player = aPlayer;
    m_Static = aStatic;
}

void CollisionManager::CheckCollisions()
{
    oxygine::Vector2 intersectionPoint;
    if (Intersection(
        oxygine::Vector2(m_Static->getX(), m_Static->getY() + m_Static->getHeight()),
        oxygine::Vector2(m_Static->getX() + m_Static->getWidth(), m_Static->getY()),
        oxygine::Vector2(m_Player->GetX(), m_Player->GetY() + m_Player->GetHeight()),
        oxygine::Vector2(m_Player->GetX(), m_Player->GetY() + m_Player->GetHeight()) + m_Player->GetDirection(),
        intersectionPoint))
    {
        std::cout << "Collision took place! " << intersectionPoint.x << ":" << intersectionPoint.y << std::endl;
        float newPosY = intersectionPoint.y - (m_Player->GetY() + m_Player->GetHeight());
        std::cout << "NewPosY:" << newPosY << std::endl;
        m_Player->SetY(newPosY);
        m_Player->SetCollisionNormal(oxygine::Vector2(0, -1));
    }



//    if (m_Player->GetX() < m_Static->getX() + m_Static->getWidth()
//        && m_Player->GetX() + m_Player->GetWidth() > m_Static->getX()
//        && m_Player->GetY() < m_Static->getY() + m_Static->getHeight()
//        && m_Player->GetY() + m_Player->GetHeight() > m_Static->getY())
//    {
//        // Collision.
//        std::cout << "Collision took place!" << std::endl;
//        m_Player->SetY(0);
//    }
    else
    {
        std::cout << m_Player->GetX() << ":" << m_Player->GetY() << " | " << m_Player->GetWidth() << ":" << m_Player->GetHeight()
                  << ";   " << m_Static->getX() << ":" << m_Static->getY() << " | " << m_Static->getWidth() << ":" << m_Static->getHeight()
                  << std::endl;

        m_Player->SetCollisionNormal(oxygine::Vector2(0, 1));
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
