#include <iostream>

#include "res.hpp"

#include "DynamicBox.hpp"

DynamicBox::DynamicBox()
{
    m_Box->setResAnim(res::ui.getResAnim("square"));
    m_Position.set(50'000, 45'000);
    m_View->setPosition(Service::Convert(m_Position));
    m_View->setSize(m_Box->getSize());
    addChild(m_View);
    m_Direction.setZero();
    SetRays();
}

DynamicBox::~DynamicBox()
{
    m_Passengers->clear();
}

void DynamicBox::Update()
{
    ICarrier::ClearPassengers();
    m_Direction.y += Service::Constants::GRAVITY;
    UpdateRays();
}

void DynamicBox::doRender(const oxygine::RenderState& a_State)
{
    DrawCollisionRays(m_Rays, a_State.transform);
    DrawCollisionRays(m_CarrierRays, a_State.transform, oxygine::Color::Blue);
}

void DynamicBox::SetPosition()
{
//    std::cout << "DynamicBox:" << m_View->getPosition().x << ":" << m_View->getPosition().y << std::endl;

//    std::cout << "In Set. DynamicBox:" << m_Direction.x << ":" << m_Direction.y << " ; Pos:" << m_Position.x << ":" << m_Position.y << std::endl;
    // Reseting direction, if collision in place.
    if ((m_Direction.x < 0 && m_CollisionNormal.x < 0) || (m_Direction.x > 0  && m_CollisionNormal.x > 0))
    {
        m_Direction.x = 0;
        std::cout << "Reseting direction x!!!" << std::endl;
    }

    ICarrier::SetPosition();

    if (m_DebugDraw)
    {
        UpdateRays();
    }

//    std::cout << "DynamicBox:" << GetX() << ":" << GetY() << std::endl;
}

void DynamicBox::CheckCollisions()
{
    IMovable::CheckCollisions();
    ICarrier::MovePassengers();
}
