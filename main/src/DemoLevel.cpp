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

void DemoLevel::init(spEventProxy aEventProxy)
{
    //create background
    // TODO : [4]
//    spSprite sky = new Sprite;
//    sky->setResAnim(res::ui.getResAnim("sky"));
//    sky->attachTo(this);

    setSize(getStage()->getSize());

    _world = new b2World(b2Vec2(0, 10));

    //create player ship
    _player = new Player;
    _player->Init(this, aEventProxy);

    //create virtual joystick
    _move = new Joystick;
    _move->attachTo(this);
    _move->setY(getHeight() - _move->getHeight());

    spButton btn = new Button;
    btn->setX(getWidth() - btn->getWidth() - 3);
    btn->setY(3);
    btn->attachTo(this);
    btn->addEventListener(TouchEvent::CLICK, CLOSURE(this, &DemoLevel::showHideDebug));

    spStatic ground = new Static(_world, RectF(getWidth() / 2, getHeight() - 10, getWidth() - 100, 30));
    addChild(ground);

    addEventListener(TouchEvent::CLICK, CLOSURE(this, &DemoLevel::click));
}

void DemoLevel::doUpdate(const UpdateState& us)
{
    //update player each frame
    //in real project you should make steps with fixed dt, check box2d documentation
     _world->Step(us.dt / 1000.0f, 6, 2);

     _player->Update(us);

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

void DemoLevel::showHideDebug(Event* event)
{
    TouchEvent* te = safeCast<TouchEvent*>(event);
    te->stopsImmediatePropagation = true;
    if (_debugDraw)
    {
        _debugDraw->detach();
        _debugDraw = 0;
        return;
    }

    _debugDraw = new Box2DDraw;
    _debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
    _debugDraw->attachTo(this);
    _debugDraw->setWorld(SCALE, _world);
    _debugDraw->setPriority(1);
}

void DemoLevel::click(Event* event)
{
    TouchEvent* te = safeCast<TouchEvent*>(event);

    if (event->target.get() == this)
    {
        spCircle circle = new Circle(_world, te->localPosition);
        circle->attachTo(this);
        _circles.push_front(circle);
    }
}
