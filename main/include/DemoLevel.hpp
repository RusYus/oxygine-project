#pragma once

#include "oxygine-framework.h"
#include "res.hpp"

#include "BasisObject.hpp"
#include "MapProperty.hpp"
#include "Box2D/Box2D.h"
#include "Utils.hpp"
#include <forward_list>

DECLARE_SMART(Circle, spCircle);
DECLARE_SMART(Square, spSquare);
DECLARE_SMART(SquareMovable, spSquareMovable);
DECLARE_SMART(Static, spStatic);
DECLARE_SMART(DemoLevel, spDemoLevel);

class Circle : public Sprite, public Basis::BasisObject
{
public:
    Circle(b2World*, const Vector2&, float);
    void Update();
    bool IsAlive = true;

public:
    b2Body* mBody;
    std::pair<Service::ObjectType, Circle*> mBodyPair;
};

class Square : public Sprite, public Basis::BasisObject
{
public:
    Square(b2World*, const Vector2&, float);
    void Update();
    bool IsAlive = true;

public:
    b2Body* mBody;
    std::pair<Service::ObjectType, Square*> mBodyPair;
};

class SquareMovable : public Sprite, public Basis::BasisObject
{
public:
    SquareMovable(b2World*, const Vector2&, float);
    void Update();
    bool IsAlive = true;

public:
    b2Body* mBody;
    std::pair<Service::ObjectType, SquareMovable*> mBodyPair;
};

class Static : public Box9Sprite, public Basis::BasisObject
{
public:
    Static(b2World*, const RectF&);

public:
    std::pair<Service::ObjectType, Static*> mBodyPair;
};

class Ground : public Basis::BasisObject
{
public:
    Ground(b2World*, const RectF&);

public:
    std::pair<Service::ObjectType, Ground*> mBodyPair;
};

class DemoLevel: public Actor
{
public:
    void Init(b2World*, MapProperty&&);
    void click(Event*);
    void showHideDebug(Event*);

private:
    friend class Player;
    void doUpdate(const UpdateState&);
    void doRender(const RenderState&);
    void drawLayer(int, int, int, int);
    void CreateTileSetTexture(Image&);

    b2World* mWorld;
    spNativeTexture mMapTexture;
    MapProperty mMapProperty;
    std::forward_list<spCircle> mCircles;
    std::forward_list<spSquareMovable> mSquares;
//    std::vector<std::unique_ptr<Ground>> mObjects;
    std::vector<spStatic> mObjects;
};
