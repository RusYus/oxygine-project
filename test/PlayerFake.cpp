#include <boost/test/unit_test.hpp>

#include "PlayerFake.hpp"

PlayerFake::PlayerFake(int a_Width, int a_Height)
    : IMovable(nullptr)
    , m_WasCollision(false)
    , m_NewDirection(0, 0)
{
    m_View->setWidth(a_Width);
    m_View->setHeight(a_Height);
    m_Direction = Service::Vector2L(0, 0);
}

void PlayerFake::SetupValues(int a_X, int a_Y, const Service::Vector2L& a_Direction)
{
    m_WasCollision = false;
    m_View->setPosition(a_X, a_Y);
    m_Direction = a_Direction;
    SetRays();
    UpdateRays();
}
