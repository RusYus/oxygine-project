#pragma once

#include "BasisObject.hpp"
#include "Box2D/Box2D.h"
#include "oxygine-framework.h"

#include <forward_list>

using namespace oxygine;

DECLARE_SMART(Circle, spCircle);
DECLARE_SMART(Square, spSquare);
DECLARE_SMART(Static, spStatic);
DECLARE_SMART(DemoLevel, spDemoLevel);

class Circle : public Sprite, public BasisObject
{
public:
    Circle(b2World*, const Vector2&, float);
    void Update();
    bool IsAlive = true;

public:
    b2Body* _body;
    std::pair<ObjectType, Circle*> _bodyPair;
};

class Square : public Sprite, public BasisObject
{
public:
    Square(b2World*, const Vector2&, float);
    void Update();
    bool IsAlive = true;

public:
    b2Body* _body;
    std::pair<ObjectType, Square*> _bodyPair;
};

class Static : public Box9Sprite, public BasisObject
{
public:
    Static(b2World*, const RectF&);

public:
    std::pair<ObjectType, Static*> _bodyPair;
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
    std::forward_list<spSquare> _squares;
};
