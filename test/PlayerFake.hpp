#include <boost/test/unit_test.hpp>

#include "Actor.h"

#include "IMovable.hpp"

struct PlayerFake : public IMovable
{
public:
    PlayerFake(int /*a_Width*/, int /*a_Height*/);

    void SetupValues(int /*a_X*/, int /*a_Y*/, const oxygine::Vector2& /*a_Direction*/);

    void SetDirection(const oxygine::Vector2& a_NewDirection) override
    {
        m_NewDirection = a_NewDirection;
    }

    void ResetCollisionNormal(const Collision::CollisionInfo& a_Sides) override
    {
        m_WasCollision = a_Sides.Down || a_Sides.Left || a_Sides.Right || a_Sides.Up;
    }

    inline bool CollisionTookPlace() const
    {
        // I presume, that in case of collision new position took place (surface of a body).
        return m_Direction != m_NewDirection || m_WasCollision;
    }
public:
    bool m_WasCollision;
    oxygine::Vector2 m_NewDirection;
};
