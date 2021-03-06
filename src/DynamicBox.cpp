#include <iostream>

#include "res.hpp"

#include "DynamicBox.hpp"

DynamicBox::DynamicBox(const oxygine::Rect& aRect, const std::shared_ptr<ICollisionManager>& a_Manager)
    : ICarrier(a_Manager)
    , IMovable(a_Manager)
{
    std::cout << "DynamicBox ID:" << GetId() << std::endl;
    Type = Service::ObjectType::DynamicBody;
    m_Box->setResAnim(res::ui.getResAnim("square"));
    m_Position.set(aRect.getX(), aRect.getY());
    m_View->setPosition(Service::Convert(m_Position));
    m_Box->setSize(Service::Convert(aRect.getSize()));
    m_View->setSize(m_Box->getSize());
    addChild(m_View);
    m_Direction.setZero();
    SetRays();
    CarrierInfo.Id = m_Id * 100;
}

DynamicBox::~DynamicBox()
{
    m_Passengers->clear();
}

void DynamicBox::Update()
{
//    ICarrier::ClearPassengers();
    // TODO : Store last directionX, restore and decrease it in update
    // So this simulate inertia (e.g. force push)
    m_Direction.x = 0;
    m_Direction.y += Service::Constants::GRAVITY;
    UpdateRays();
//    std::cout << "DynamicBox:" << m_Direction << "; Pos:" << m_Position << std::endl;
}

void DynamicBox::doRender(const oxygine::RenderState& a_State)
{
    DrawCollisionRays(m_Rays, a_State.transform);
    DrawCollisionRays(m_CarrierRays, a_State.transform, oxygine::Color::Blue);
}

void DynamicBox::SetPosition()
{
//    std::cout << "DynamicBox:" << m_View->getPosition().x << ":" << m_View->getPosition().y << std::endl;

//    std::cout << "In Set. DynamicBox:" << m_Direction << "; Pos:" << m_Position << std::endl;
    // Reseting direction, if collision in place.
    if ((m_Direction.x < 0 && m_CollisionNormal.x < 0) || (m_Direction.x > 0  && m_CollisionNormal.x > 0))
    {
        m_Direction.x = 0;
        std::cout << "DynamicBox:Reseting direction x!!!" << std::endl;
    }

    ICarrier::SetPosition();

    if (m_DebugDraw)
    {
        UpdateRays();
    }

//    std::cout << "DynamicBox:" << GetX() << ":" << GetY() << std::endl;
}

//void DynamicBox::CheckCollisions()
//{
//    IMovable::CheckCollisions();
//    ICarrier::MovePassengers();
//}

void DynamicBox::AddDirection(const Service::Vector2L& a_Direction)
{
    IMovable::AddDirection(a_Direction);
//    std::cout << "Added direction, new:" << m_Direction << std::endl;
}
