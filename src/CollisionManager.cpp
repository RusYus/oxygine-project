#include <limits>

#include "CollisionManager.hpp"

void CollisionManager::CheckCollisions(Basis::BasisObject::TId a_Id)
{
    // TODO : Optimizations checks for collisions (quad tree or four-areas on screen?).

    auto bodyIt = m_Bodies.find(a_Id);
    if (bodyIt == m_Bodies.end())
    {
        return;
    }

    Collision::CollisionInfo collisionSides;

    m_Rectangle.Width = 0;
    m_Rectangle.Height = 0;
    m_Rectangle.bottomLeft.set(std::numeric_limits<Service::TCoordinate>::quiet_NaN(), std::numeric_limits<Service::TCoordinate>::quiet_NaN());
    m_Rectangle.topRight.set(std::numeric_limits<Service::TCoordinate>::quiet_NaN(), std::numeric_limits<Service::TCoordinate>::quiet_NaN());
    collisionSides.Reset();

    IMovable* body = dynamic_cast<IMovable*>(bodyIt->second.first);
    Service::Vector2L intersectionPoint;
    Service::Vector2L newDirection = body->GetDirection();

    for (auto& secondBodyIt : m_Bodies)
    {
        Basis::BasisObject* secondBody = secondBodyIt.second.first;
        if (!secondBody)
        {
            std::cout << "No second body!" << std::endl;
            continue;
        }

        // Don't check collisions if same body or it's carrier.
        if (body->GetId() == secondBody->GetId()
            || body->CarrierId == secondBody->GetId())
        {
            continue;
        }

        // Setting collision boundaries for second body.
        FillRectangleValues(*secondBody);

        if (body->Type == Service::ObjectType::DynamicBody || body->Type == Service::ObjectType::Platform)
        {
            if (secondBody->Type == Service::ObjectType::DynamicBody || secondBody->Type == Service::ObjectType::Player)
            {
                IMovable* possiblePassenger = dynamic_cast<IMovable*>(secondBody);
                ICarrier* carrier = dynamic_cast<ICarrier*>(body);

                UpdateRectangleWithDirection(*possiblePassenger);

                Service::Vector2L additionalDirection{possiblePassenger->GetDirection().x, 0};
                if (HandleCarrierIntersection(carrier, additionalDirection))
                {
                    // If passenger is Player and is jumping, then ignore collision.
                    if (dynamic_cast<Player*>(possiblePassenger) && dynamic_cast<Player*>(possiblePassenger)->IsJumping())
                    {
                        continue;
                    }

                    // Set new direction to passenger (after first collision it's suposed to be 0).
                    possiblePassenger->SetDirection(additionalDirection);
                    carrier->AddPassenger(possiblePassenger);
                    continue;
                }
            }
        }

        if (body->Type == Service::ObjectType::DynamicBody || body->Type == Service::ObjectType::Player)
        {
            HandleIntersection(body, collisionSides, intersectionPoint, newDirection);
        }
    }

    if (newDirection != body->GetDirection())
    {
        body->SetDirection(newDirection);
    }

    body->ResetCollisionNormal(collisionSides);
}

void CollisionManager::FillRectangleValues(Basis::BasisObject& a_out_Body)
{
    m_Rectangle.Width = a_out_Body.GetWidth();
    m_Rectangle.Height = a_out_Body.GetHeight();
    m_Rectangle.bottomLeft.set(a_out_Body.GetX(), a_out_Body.GetY() + a_out_Body.GetHeight());
    m_Rectangle.topRight.set(a_out_Body.GetX() + a_out_Body.GetWidth(), a_out_Body.GetY());
}

void CollisionManager::UpdateRectangleWithDirection(IMovable& a_out_Body)
{
    if (a_out_Body.GetDirection().x >= 0)
    {
        m_Rectangle.topRight.x += a_out_Body.GetDirection().x;
    }
    else
    {
        m_Rectangle.bottomLeft.x += a_out_Body.GetDirection().x;
    }
    if (a_out_Body.GetDirection().y >= 0)
    {
        m_Rectangle.bottomLeft.y += a_out_Body.GetDirection().y;
    }
    else
    {
        m_Rectangle.topRight.y += a_out_Body.GetDirection().y;
    }
}

// TODO : Use better names, more comments.
bool CollisionManager::Intersection(
        const Service::Vector2L& aBottomLeftAABB, const Service::Vector2L& aTopRightAABB,
        const Service::Vector2L& aStartRay, const Service::Vector2L& aEndRay,
        Service::Vector2L& outIntersection)
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

    Service::Vector2L b = aEndRay - aStartRay;


    outIntersection = aStartRay + b * f_low;
    // Check if outIntersection == aStartRay. In this case aEndRay must be outside of aabb (not even on the border)
    // For example, when jumping, end ray points outside of aabb (so there will be no collision).
    // As opposite of when standing still, where end ray points into aabb.

    if (outIntersection == aStartRay)
    {
        return (aBottomLeftAABB.x < aEndRay.x && aEndRay.x < aTopRightAABB.x && aTopRightAABB.y < aEndRay.y && aEndRay.y < aBottomLeftAABB.y);
    }

    return true;

}
