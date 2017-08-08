#include <iostream>

#include "res.hpp"

#include "Platform.hpp"

Platform::Platform(const oxygine::RectF& aRect)
    : m_BodyPair(Service::ObjectType::Ground, this)
{
    setResAnim(res::ui.getResAnim("platform"));
    setSize(aRect.getSize());
//    setPosition(aRect.getCenter());
    setPosition(aRect.getLeftTop());
    setAnchor(Vector2(0.5f, 0.5f));

    // TODO : Read from config.

    PathNode newPoint = PathNode(0, oxygine::Vector2(4, 2));
    m_Nodes.emplace(std::make_pair(newPoint.Id, newPoint));
    PathNode newPoint2 = PathNode(1, oxygine::Vector2(6, 2));
    m_Nodes.emplace(std::make_pair(newPoint2.Id, newPoint2));
    PathNode newPoint3 = PathNode(2, oxygine::Vector2(6, 0.5));
    m_Nodes.emplace(std::make_pair(newPoint3.Id, newPoint3));
    PathNode newPoint4 = PathNode(3, oxygine::Vector2(9, 0.5));
    m_Nodes.emplace(std::make_pair(newPoint4.Id, newPoint4));


    m_Direction = oxygine::Vector2(m_Speed, 0);
}

bool Platform::IsAroundNode()
{
//    const b2Vec2 currentPosition = m_Body->GetPosition();

//    // TODO : better compare mechanism for high velocity.
////    return (std::abs(currentPosition.x - m_Nodes.at(m_NextNodeId).Position.x) <= NODE_SLIPPAGE)
////        && (std::abs(currentPosition.y - m_Nodes.at(m_NextNodeId).Position.y) <= NODE_SLIPPAGE);
//    return std::abs(b2Distance(currentPosition, m_Nodes.at(m_NextNodeId).Position)) < NODE_SLIPPAGE;
    return true;
}

void Platform::Move()
{
    if (IsAroundNode())
    {
        auto currentId = m_NextNodeId;

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

//        m_Direction = b2Vec2(m_Nodes.at(m_NextNodeId).Position - m_Nodes.at(currentId).Position);
//        m_Direction.Normalize();
//        m_Direction *= m_Speed;
    }
}
