#include <boost/test/unit_test.hpp>

#include "PlayerFake.hpp"

PlayerFake::PlayerFake(const std::shared_ptr<ICollisionManager>& a_Manager)
    : IMovable(a_Manager)
    , m_WasCollision(false)
    , m_InitialDirection(0, 0)
{
    Type = Service::ObjectType::Player;
    m_View->setPosition(0, 0);
    m_View->setSize(oxygine::Vector2(100, 100));
    m_Direction.setZero();
    SetRays();
}

void PlayerFake::SetupValues(int a_X, int a_Y, const Service::Vector2L& a_Direction)
{
    m_WasCollision = false;
    m_Position.set(a_X, a_Y);
    m_View->setPosition(Service::Convert(m_Position));
    m_View->setSize(oxygine::Vector2(100, 100));
    m_Direction.set(a_Direction.x, a_Direction.y);
    m_InitialDirection.set(a_Direction.x, a_Direction.y);
    SetRays();
    UpdateRays();
}
