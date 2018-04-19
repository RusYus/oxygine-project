#include <boost/test/unit_test.hpp>

#include "Actor.h"

#include "IMovable.hpp"

struct PlayerFake : public IMovable
{
public:
    PlayerFake(const std::shared_ptr<ICollisionManager>&);

    void SetupValues(int /*a_X*/, int /*a_Y*/, const Service::Vector2L& /*a_Direction*/);

    void ResetCollisionNormal(const Collision::CollisionInfo& a_Sides) override
    {
        m_WasCollision = a_Sides.Down || a_Sides.Left || a_Sides.Right || a_Sides.Up;
    }

    inline Service::TCoordinate GetWidth() const override
    {
        return Service::TCoordinate(10'000);
    }

    inline Service::TCoordinate GetHeight() const override
    {
        return Service::TCoordinate(10'000);
    }

    inline bool CollisionTookPlace() const
    {
        // I presume, that in case of collision new position took place (surface of a body).
        return m_Direction != m_InitialDirection || m_WasCollision;
    }
public:
    bool m_WasCollision;
    Service::Vector2L m_InitialDirection;
};
