#pragma once

#include <forward_list>

#include "Box2D/Box2D.h"
#include "Box2DDebugDraw.h"
#include "oxygine-framework.h"
using namespace oxygine;

DECLARE_SMART(Player, spPlayer);
DECLARE_SMART(Joystick, spJoystick);
DECLARE_SMART(Circle, spCircle);
DECLARE_SMART(Static, spStatic);
DECLARE_SMART(Game, spGame);

class Circle : public Sprite
{
public:
    Circle(b2World*, const Vector2&, float);
    void Update();
    bool IsAlive = true;

public:
    b2Body* _body;
};

class Static : public Box9Sprite
{
public:
    Static(b2World*, const RectF&);
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

    std::forward_list<spCircle> _circles;
};
