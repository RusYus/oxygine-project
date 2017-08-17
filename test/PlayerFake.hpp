#include <boost/test/unit_test.hpp>

#include "Actor.h"

#include "IMovable.hpp"

// TODO : Move realization to cpp.

struct PlayerFake : public IMovable
{
public:
    PlayerFake(int a_Width, int a_Height)
        : m_WasCollision(false)
        , m_NewDirection(0, 0)
    {
        m_View->setWidth(a_Width);
        m_View->setHeight(a_Height);
        m_Direction = oxygine::Vector2(0, 0);
    }

    void SetupValues(int a_X, int a_Y, const oxygine::Vector2& a_Direction)
    {
        m_WasCollision = false;
        m_View->setPosition(a_X, a_Y);
        m_Direction = a_Direction;
        SetRays();
        UpdateRays();
    }

    void SetDirection(const oxygine::Vector2& a_NewDirection, bool a_SetExact = false) override
    {
        m_NewDirection = a_NewDirection;
    }

    void SetCollisionNormal(const oxygine::Vector2) override {}
    void ResetCollisionNormal(const Collision::CollisionInfo& a_Sides) override
    {
        m_WasCollision = a_Sides.Down || a_Sides.Left || a_Sides.Right || a_Sides.Up;
    }

    bool CollisionTookPlace() const
    {
        // I presume, that in case of collision new position took place (surface of a body).
        return m_Direction != m_NewDirection || m_WasCollision;
    }
public:
    bool m_WasCollision;
    oxygine::Vector2 m_NewDirection;
};
