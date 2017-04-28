#pragma once
#include "Box2D/Box2D.h"
#include "Box2DDebugDraw.h"
#include "oxygine-framework.h"
using namespace oxygine;

DECLARE_SMART(Player, spPlayer);
DECLARE_SMART(Joystick, spJoystick);
DECLARE_SMART(Circle, spCircle);
DECLARE_SMART(Static, spStatic);
DECLARE_SMART(Game, spGame);

class PlayerListener : public b2ContactListener
{
    virtual void BeginContact(b2Contact* contact) override;
    virtual void EndContact(b2Contact* contact) override;
};


class Circle : public Sprite
{
public:
    Circle(b2World*, const Vector2&, float);
    void Update()
{
    const b2Vec2& pos = _body->GetPosition();
    setPosition(Vector2(pos.x * 100, pos.y * 100));
    setRotation(_body->GetAngle());
}
    bool IsAlive=true;
//    {
//        setResAnim(res::ui.getResAnim("circle"));
//        setAnchor(Vector2(0.5f, 0.5f));
//        setTouchChildrenEnabled(false);

//        b2BodyDef bodyDef;
//        bodyDef.type = b2_dynamicBody;
//        bodyDef.position = convert(pos);

//        b2Body* body = world->CreateBody(&bodyDef);

//        setUserData(body);

//        setScale(scale);

//        b2CircleShape shape;
//        shape.m_radius = getWidth() / SCALE / 2 * scale;

//        b2FixtureDef fixtureDef;
//        fixtureDef.shape = &shape;
//        fixtureDef.density = 1.0f;
//        fixtureDef.friction = 0.3f;

//        body->CreateFixture(&fixtureDef);
//        body->SetUserData(this);
//    }
    b2Body* _body;
};

class Static : public Box9Sprite
{
public:
    Static(b2World*, const RectF&);
//    {
//        //setHorizontalMode(Box9Sprite::TILING_FULL);
//        //setVerticalMode(Box9Sprite::TILING_FULL);
//        setResAnim(res::ui.getResAnim("pen"));
//        setSize(rc.getSize());
//        setPosition(rc.getLeftTop());
//        setAnchor(Vector2(0.5f, 0.5f));

//        b2BodyDef groundBodyDef;
//        groundBodyDef.position = convert(getPosition());

//        b2Body* groundBody = world->CreateBody(&groundBodyDef);

//        b2PolygonShape groundBox;
//        b2Vec2 sz = convert(getSize() / 2);
//        groundBox.SetAsBox(sz.x, sz.y);
//        groundBody->CreateFixture(&groundBox, 0.0f);
//    }
};

class Game: public Actor
{
public:
    Game();

    void init();

    void click(Event*);
    void showHideDebug(Event*);

private:
    friend class Player;
    void doUpdate(const UpdateState& us);

    spJoystick _move;
    spPlayer _player;

    b2World* _world;
    spBox2DDraw _debugDraw;
    PlayerListener _pl;

    std::vector<spCircle> _circles;

    bool b = false;
};
