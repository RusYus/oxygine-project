#include <iostream>

#include "res.hpp"

#include "Platform.hpp"

Platform::Platform(b2World* aWorld, const oxygine::RectF& aRect)
    : m_BodyPair(Service::ObjectType::Ground, this)
{
    setResAnim(res::ui.getResAnim("platform"));
    setSize(aRect.getSize());
    setPosition(aRect.getCenter());
    setAnchor(Vector2(0.5f, 0.5f));

    // TODO : Read from config.

    PathNode newPoint = PathNode(0, b2Vec2(4, 1.7));
    m_Nodes.emplace(std::make_pair(newPoint.Id, newPoint));
    PathNode newPoint2 = PathNode(1, b2Vec2(7, 1.7));
    m_Nodes.emplace(std::make_pair(newPoint2.Id, newPoint2));
    PathNode newPoint3 = PathNode(2, b2Vec2(7, 0.1));
    m_Nodes.emplace(std::make_pair(newPoint3.Id, newPoint3));


    m_Direction = b2Vec2(4, 0);

    b2BodyDef bodyDef;
    bodyDef.fixedRotation = true;
    bodyDef.type = b2_kinematicBody;
    bodyDef.position = Service::Utils::Convert(getPosition());

    m_Body = aWorld->CreateBody(&bodyDef);

    b2PolygonShape box;
    b2Vec2 size = Service::Utils::Convert(aRect.getSize() / 2);
    box.SetAsBox(size.x, size.y);

    b2Filter filter;
    filter.categoryBits = 0x0001;
    filter.maskBits = 0x0003;
    filter.groupIndex = 3;

    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.0f;
    fixtureDef.shape = &box;
    fixtureDef.filter = filter;
    m_Body->CreateFixture(&fixtureDef);
    m_Body->SetUserData(&m_BodyPair);
}

bool Platform::IsAroundNode()
{
    const b2Vec2 currentPosition = m_Body->GetPosition();

    return (std::abs(currentPosition.x - m_Nodes.at(m_NextNodeId).Position.x) < m_Speed * NODE_SLIPPAGE)
        && (std::abs(currentPosition.y - m_Nodes.at(m_NextNodeId).Position.y) < m_Speed * NODE_SLIPPAGE);
}

void Platform::Move()
{
    const b2Vec2 currentPosition = m_Body->GetPosition();
    if (IsAroundNode())
    {
        auto currentId = m_NextNodeId;

        // Last node (by id).
        if (m_NextNodeId == 2)
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

        m_Direction = b2Vec2(m_Nodes.at(m_NextNodeId).Position - m_Nodes.at(currentId).Position);
        m_Direction.Normalize();
        m_Direction *= 4;
    }

    m_Body->SetLinearVelocity(m_Direction);
    setPosition(Vector2(Service::Utils::Convert(m_Body->GetPosition())));
    std::cout << "Platform:" << m_Body->GetPosition().x << std::endl;
//        std::cout << "Platform:" << std::round(getPosition().x) << ", " << std::round(getPosition().y)
//                     <<":"
//                  <<  std::round( m_Points.at(m_NextPointId).Position.x)
//                   << ", "  <<  std::round( m_Points.at(m_NextPointId).Position.y)
//                  << std::endl;

}
