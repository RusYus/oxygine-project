#include <iostream>

#include "res.hpp"

#include "Platform.hpp"

Platform::Platform(const oxygine::Rect& aRect, const std::shared_ptr<ICollisionManager>& a_Manager)
    : ICarrier(a_Manager)
    , IMovable(a_Manager)
{
    std::cout << "Platform ID:" << GetId() << std::endl;
    Type = Service::ObjectType::Platform;
    m_Box->setResAnim(res::ui.getResAnim("platform"));
    m_Position.set(60'000, 35'000);
    m_View->setPosition(Service::Convert(m_Position));
    m_Box->setSize(Service::Convert(aRect.getSize()));
    m_View->setSize(m_Box->getSize());

    addChild(m_View);
//    setAnchor(Vector2(0.5f, 0.5f));

    // TODO : Read from config.

    PathNode newPoint = PathNode(0, GetPosition());
    m_Nodes.emplace(std::make_pair(newPoint.Id, newPoint));
    PathNode newPoint2 = PathNode(1, newPoint.Position + Service::Vector2L(-40'000, 0));
    m_Nodes.emplace(std::make_pair(newPoint2.Id, newPoint2));
//    PathNode newPoint3 = PathNode(2, newPoint2.Position + Service::Vector2L(25'000, 0));
//    m_Nodes.emplace(std::make_pair(newPoint3.Id, newPoint3));
//    PathNode newPoint4 = PathNode(3, newPoint3.Position + Service::Vector2L(10'000, 15'000));
//    m_Nodes.emplace(std::make_pair(newPoint4.Id, newPoint4));
//    PathNode newPoint5 = PathNode(4, newPoint.Position + Service::Vector2L(400, 0));
//    m_Nodes.emplace(std::make_pair(newPoint5.Id, newPoint5));

    SetDirection(m_Nodes.at(m_NextNodeId).Position - m_Nodes.at(0).Position);

    SetRays();
}

Platform::~Platform()
{
    m_Passengers->clear();
}

bool Platform::IsAroundNode()
{
    const Service::Vector2L nodeDirection = m_Nodes.at(m_NextNodeId).Position - GetPosition();
    auto q1 = nodeDirection.cast<oxygine::Vector2>().length() ;
    auto q2 = GetDirection().cast<oxygine::Vector2>().length();
    return q1<=q2;
}

void Platform::Update()
{
    ICarrier::ClearPassengers();

    if (IsAroundNode())
    {
        ICarrier::SetDirection(m_Nodes.at(m_NextNodeId).Position - GetPosition());

        // Last node (by id).
        if (m_NextNodeId == static_cast<PathNode::TId>(m_Nodes.size() - 1))
        {
            if (m_RunningMode == PointToPointMode::BackToBack)
            {
                m_NextNodeId--;
                m_IsMovingReverse = true;
            }
            else
            {
                m_NextNodeId = 0;
            }
        }
        // First node (by id).
        else if (m_NextNodeId == 0)
        {
            if (m_RunningMode == PointToPointMode::BackToBack)
            {
                m_NextNodeId++;
                m_IsMovingReverse = false;
            }
            else
            {
                m_NextNodeId++;
            }
        }
        else
        {
            if (m_RunningMode == PointToPointMode::BackToBack)
            {
                m_NextNodeId += m_IsMovingReverse ? -1 : 1;
            }
            else
            {
                m_NextNodeId++;
            }
        }
    }
    else
    {
        SetDirection(m_Nodes.at(m_NextNodeId).Position - GetPosition());
    }

//    std::cout << "Platform:-----------------" << std::endl;
    UpdateRays();
}

void Platform::doRender(const oxygine::RenderState& a_State)
{
    DrawCollisionRays(m_Rays, a_State.transform);
    DrawCollisionRays(m_CarrierRays, a_State.transform, oxygine::Color::Blue);
}

void Platform::SetDirection(const Service::Vector2L& a_NewDirection)
{
    ICarrier::SetDirection(a_NewDirection);

    if (a_NewDirection != Service::Vector2L(0, 0))
    {
        oxygine::Vector2 dirTemp = Service::Convert(m_Direction);
        dirTemp.normalize();
        dirTemp.x = std::round(dirTemp.x);
        dirTemp.y = std::round(dirTemp.y);
        m_Direction = Service::Convert(dirTemp);
        m_Direction *= (m_MaxSpeed / Service::Constants::SCALE);
    }
//    std::cout << "Platform:" << m_Direction.x << ":" << m_Direction.y << " ; Pos:" << m_Position.x << ":" << m_Position.y << std::endl;
}

void Platform::SetPosition()
{
//    std::cout << "Platform:" << m_View->getPosition().x << ":" << m_View->getPosition().y << std::endl;

//    std::cout << "In Set. Platform:" << m_Direction.x << ":" << m_Direction.y << " ; Pos:" << m_Position.x << ":" << m_Position.y << std::endl;
    ICarrier::SetPosition();

    if (m_DebugDraw)
    {
        UpdateRays();
    }

//    std::cout << "Platform:" << GetX() << ":" << GetY() << std::endl;
}

void Platform::CheckCollisions()
{
    IMovable::CheckCollisions();
    ICarrier::MovePassengers();
}
