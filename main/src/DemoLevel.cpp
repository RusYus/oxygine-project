#include "DemoLevel.h"
#include "Player.h"
#include "Joystick.h"
#include "res.h"
#include <iostream>

b2Vec2 convert(const Vector2& pos)
{
    return b2Vec2(pos.x / SCALE, pos.y / SCALE);
}

Vector2 convert(const b2Vec2& pos)
{
    return Vector2(pos.x * SCALE, pos.y * SCALE);
}

Circle::Circle(b2World* world, const Vector2& pos, float scale = 1)
{
    setResAnim(res::ui.getResAnim("circle"));
    setAnchor(Vector2(0.5f, 0.5f));
    setTouchChildrenEnabled(false);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = convert(pos);

    _body = world->CreateBody(&bodyDef);

    setUserData(_body);

    setScale(scale);

    b2CircleShape shape;
    shape.m_radius = getWidth() / SCALE / 2 * scale;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(this);
}

void Circle::Update()
{
    const b2Vec2& pos = _body->GetPosition();
    setPosition(Vector2(pos.x * 100, pos.y * 100));
    setRotation(_body->GetAngle());
}

Static::Static(b2World* world, const RectF& rc)
{
    setResAnim(res::ui.getResAnim("pen"));
    setSize(rc.getSize());
    setPosition(rc.getLeftTop());
    setAnchor(Vector2(0.5f, 0.5f));

    b2BodyDef groundBodyDef;
    groundBodyDef.position = convert(getPosition());

    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    b2Vec2 sz = convert(getSize() / 2);
    groundBox.SetAsBox(sz.x, sz.y);
    groundBody->CreateFixture(&groundBox, 0.0f);
}

DemoLevel::DemoLevel()
{
}

void DemoLevel::Init(b2World* aWorld)
{
    _world = aWorld;
    //create background
    // TODO : [4]
    spSprite sky = new Sprite;
    sky->setResAnim(res::ui.getResAnim("sky"));
    sky->attachTo(this);

    setSize(getStage()->getSize().x*3, getStage()->getSize().y*2);

    spStatic ground = new Static(_world, RectF(getWidth() / 2, getHeight() - 10, getWidth() - 100, 30));
    addChild(ground);

    addEventListener(TouchEvent::CLICK, CLOSURE(this, &DemoLevel::click));
}

void DemoLevel::doUpdate(const UpdateState& /*us*/)
{
    for(auto& circle : _circles)
    {
        assert(circle->IsAlive);

        circle->Update();

        if (circle->getY() > getHeight() + 50)
        {
            circle->_body->SetUserData(0);
            _world->DestroyBody(circle->_body);

            circle->detach();
            circle->IsAlive = false;
        }
    }

    _circles.remove_if([](spCircle circle) { return !circle->IsAlive; });
}

void DemoLevel::click(Event* event)
{
    std::cout << "demo level click" << std::endl;

    TouchEvent* te = safeCast<TouchEvent*>(event);

    // Background is target in this case, so removing if statement
    // for now. In future, need to read manuals for more inticate case.
//    if (event->target.get() == this)
//    {
        std::cout << "Creating circle!" << std::endl;
        spCircle circle = new Circle(_world, te->localPosition);
        circle->attachTo(this);
        _circles.push_front(circle);
//    }
}
