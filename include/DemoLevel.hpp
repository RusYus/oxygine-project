#pragma once

#include <forward_list>

#include "res.hpp"

#include "Ground.hpp"
#include "Platform.hpp"
#include "MapProperty.hpp"
#include "Utils.hpp"

DECLARE_SMART(Circle, spCircle);
DECLARE_SMART(Square, spSquare);
DECLARE_SMART(SquareMovable, spSquareMovable);
DECLARE_SMART(Ground, spGround);
DECLARE_SMART(DemoLevel, spDemoLevel);

class Circle : public Sprite, public Basis::BasisObject
{
public:
    Circle(const Vector2&, float);
    void Update();
    bool IsAlive = true;

public:
    std::pair<Service::ObjectType, Circle*> mBodyPair;
};

class Square : public Sprite, public Basis::BasisObject
{
public:
    Square(const Vector2&, float);
    void Update();
    bool IsAlive = true;

public:
    std::pair<Service::ObjectType, Square*> mBodyPair;
};

class SquareMovable : public Sprite, public Basis::BasisObject
{
public:
    SquareMovable(const Vector2&, float);
    void Update();
    bool IsAlive = true;

public:
    std::pair<Service::ObjectType, SquareMovable*> mBodyPair;
};

class Ground : public Box9Sprite, public Basis::BasisObject
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
    void Update(const UpdateState&);
    void showHideDebug(Event*);
    Static* mStatic;
    Static* mStatic2;
    Static* mStatic3;
    Static* mStatic4;
    spPlatform m_Platform;

private:
    friend class Player;
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
