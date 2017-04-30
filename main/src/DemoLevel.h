#pragma once

#include <forward_list>

#include "Box2D/Box2D.h"
#include "oxygine-framework.h"
using namespace oxygine;

DECLARE_SMART(Circle, spCircle);
DECLARE_SMART(Static, spStatic);
DECLARE_SMART(DemoLevel, spDemoLevel);

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

class DemoLevel: public Actor
{
public:
    DemoLevel();

    void Init(b2World*);
    void click(Event*);
    void showHideDebug(Event*);

private:
    friend class Player;
    void doUpdate(const UpdateState&);

    b2World* _world;

    std::forward_list<spCircle> _circles;
};
