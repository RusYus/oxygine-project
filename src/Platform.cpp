#include <iostream>

#include "res.hpp"

#include "Platform.hpp"

Platform::Platform(const oxygine::RectF& aRect)
    : m_BodyPair(Service::ObjectType::Ground, this)
    , m_DirectionUntilStop(m_Direction)
{
    m_Box->setResAnim(res::ui.getResAnim("platform"));
    m_View->setPosition(350, 450);
    m_Box->setSize(aRect.getSize());
    m_View->setSize(m_Box->getSize());

    addChild(m_View);
//    setAnchor(Vector2(0.5f, 0.5f));

    // TODO : Read from config.

    PathNode newPoint = PathNode(0, GetPosition());
    m_Nodes.emplace(std::make_pair(newPoint.Id, newPoint));
    PathNode newPoint2 = PathNode(1, newPoint.Position + oxygine::Vector2(150, 0));
    m_Nodes.emplace(std::make_pair(newPoint2.Id, newPoint2));
    PathNode newPoint3 = PathNode(2, newPoint.Position + oxygine::Vector2(150, -100));
    m_Nodes.emplace(std::make_pair(newPoint3.Id, newPoint3));
    PathNode newPoint4 = PathNode(3, newPoint.Position + oxygine::Vector2(300, -100));
    m_Nodes.emplace(std::make_pair(newPoint4.Id, newPoint4));
//    PathNode newPoint5 = PathNode(4, newPoint.Position + oxygine::Vector2(400, 0));
//    m_Nodes.emplace(std::make_pair(newPoint5.Id, newPoint5));

    SetDirection(m_Nodes.at(m_NextNodeId).Position - m_Nodes.at(0).Position);

    SetRays();
}

bool Platform::IsAroundNode()
{

//    // TODO : better compare mechanism for high velocity.
//    return (std::abs(currentPosition.x - m_Nodes.at(m_NextNodeId).Position.x) <= NODE_SLIPPAGE)
//        && (std::abs(currentPosition.y - m_Nodes.at(m_NextNodeId).Position.y) <= NODE_SLIPPAGE);

    const oxygine::Vector2 nodeDirection = m_Nodes.at(m_NextNodeId).Position - GetPosition();
//    const oxygine::Vector2 currentPosition = GetPosition() + GetDirection();
    return (nodeDirection.length() <= GetDirection().length());

//    return std::abs(b2Distance(currentPosition, m_Nodes.at(m_NextNodeId).Position)) < NODE_SLIPPAGE;

}

void Platform::Move()
{

    if (IsAroundNode())
    {
        SetDirection(m_Nodes.at(m_NextNodeId).Position - GetPosition(), true);

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

    UpdateRays();
}

void Platform::doRender(const oxygine::RenderState& a_State)
{
    DrawCollisionRays(std::make_shared<decltype(m_Rays)>(m_Rays), a_State.transform);
}

void Platform::SetDirection(const Vector2& a_NewDirection, bool a_SetExact)
{
    if (a_NewDirection != oxygine::Vector2(0, 0))
    {
        IMovable::SetDirection(a_NewDirection);

        if (!a_SetExact)
        {
            m_Direction.normalize();
            m_Direction *= m_MaxSpeed;
//            Service::RoundToOneDigit(m_Direction);
            m_DirectionUntilStop = m_Direction;
        }

        if (m_Direction.x != 0 && m_Direction.x != 3)
        {
            std::cout << std::endl;
        }
    }
    else
    {
        m_Direction.setZero();
    }
}

void Platform::ResetCollisionNormal(const Collision::CollisionInfo& a_Sides)
{
    IMovable::ResetCollisionNormal(a_Sides);

    if (m_CollisionNormal.x == 0 && m_DirectionUntilStop.x != 0)
    {
        m_Direction.x = m_DirectionUntilStop.x;
    }

    if (m_CollisionNormal.y == 0 && m_DirectionUntilStop.y != 0)
    {
        m_Direction.y = m_DirectionUntilStop.y;
    }
}

void Platform::SetPosition()
{
    IMovable::SetPosition();

    // TODO : Refactor!
    if (m_DebugDraw)
    {
        UpdateRays();
    }

    std::cout << m_Direction.x << ":" << m_Direction.y << std::endl;
}
