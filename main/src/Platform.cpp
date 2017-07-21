#include "res.hpp"

#include "Platform.hpp"

Platform::Platform(b2World* aWorld, const oxygine::RectF& aRect)
    : m_BodyPair(Service::ObjectType::Ground, this)
{
    setResAnim(res::ui.getResAnim("platform"));
    setSize(aRect.getSize());
    setPosition(aRect.getCenter());
    setAnchor(Vector2(0.5f, 0.5f));

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

void Platform::Move(b2Vec2 aDirection)
{
    m_Body->SetLinearVelocity(aDirection);
    setPosition(Vector2(Service::Utils::Convert(m_Body->GetPosition())));
}
