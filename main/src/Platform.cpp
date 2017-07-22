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

//    for (unsigned short i = 0 ; i < 2; ++i)
//    {
//        PlatformPoint newPoint = PlatformPoint(i, Vector2(getPosition().x + i * 300, getPosition().y));
//        m_Points.emplace(std::make_pair(newPoint.Id, newPoint));
//    }

    PlatformPoint newPoint = PlatformPoint(0, Vector2(400, 170));
    m_Points.emplace(std::make_pair(newPoint.Id, newPoint));
    PlatformPoint newPoint2 = PlatformPoint(1, Vector2(700, 170));
    m_Points.emplace(std::make_pair(newPoint2.Id, newPoint2));
    PlatformPoint newPoint3 = PlatformPoint(2, Vector2(700, 10));
    m_Points.emplace(std::make_pair(newPoint3.Id, newPoint3));


    m_Direction = b2Vec2(0.6, 0);

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

void Platform::Move()
{
    const Vector2 currentPosition = getPosition();
    if (std::round(getPosition().x) == std::round( m_Points.at(m_NextPointId).Position.x)
        && std::round(getPosition().y) == std::round( m_Points.at(m_NextPointId).Position.y))
    {
        auto currentId = m_NextPointId;

        // Max
        if (m_NextPointId == 2)
        {
            if (m_RunningMode == PointToPointMode::BackToBack)
            {
                m_NextPointId--;
                m_IsMovingReverse = true;
            }
            else
            {
                m_NextPointId = 0;
            }
        }
        // Min
        else if (m_NextPointId == 0)
        {
            if (m_RunningMode == PointToPointMode::BackToBack)
            {
                m_NextPointId++;
                m_IsMovingReverse = false;
            }
            else
            {
                m_NextPointId++;
            }
        }
        else
        {
            if (m_RunningMode == PointToPointMode::BackToBack)
            {
                m_NextPointId += m_IsMovingReverse ? -1 : 1;
            }
            else
            {
                m_NextPointId++;
            }
        }

        Vector2 newDirection = Vector2(m_Points.at(m_NextPointId).Position - m_Points.at(currentId).Position);
        newDirection.normalize();
        newDirection *= 60;
        m_Direction = Service::Utils::Convert(newDirection);
        m_Body->SetLinearVelocity(m_Direction);
        setPosition(newDirection);
    }
    else
    {
        m_Body->SetLinearVelocity(m_Direction);
        setPosition(Vector2(Service::Utils::Convert(m_Body->GetPosition())));
//        std::cout << "Platform:" << m_Direction.x << std::endl;
        std::cout << "Platform:" << std::round(getPosition().x) << ", " << std::round(getPosition().y)
                     <<":"
                  <<  std::round( m_Points.at(m_NextPointId).Position.x)
                   << ", "  <<  std::round( m_Points.at(m_NextPointId).Position.y)
                  << std::endl;
    }

}
