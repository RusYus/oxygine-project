#pragma once

#include <forward_list>

#include "res.hpp"

#include "BasisObject.hpp"
#include "Platform.hpp"
#include "MapProperty.hpp"
#include "Utils.hpp"

DECLARE_SMART(Circle, spCircle);
DECLARE_SMART(Square, spSquare);
DECLARE_SMART(SquareMovable, spSquareMovable);
DECLARE_SMART(Static, spStatic);
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

class Static : public Box9Sprite, public Basis::BasisObject
{
public:
    Static(const RectF&);

public:
    std::pair<Service::ObjectType, Static*> mBodyPair;
};

class Ground : public Basis::BasisObject
{
public:
    Ground(const RectF&);

public:
    std::pair<Service::ObjectType, Ground*> mBodyPair;
};

class DemoLevel: public Actor
{
public:
    void Init(MapProperty&&);
    void click(Event*);
    void showHideDebug(Event*);
    Static* mStatic;

private:
    friend class Player;
    void doUpdate(const UpdateState&);
    void doRender(const RenderState&);
    void drawLayer(int, int, int, int);
    void CreateTileSetTexture(Image&);

    spNativeTexture mMapTexture;
    MapProperty mMapProperty;
    std::forward_list<spCircle> mCircles;
    std::forward_list<spSquareMovable> mSquares;
    spPlatform m_Platform;
//    std::vector<std::unique_ptr<Ground>> mObjects;
    std::vector<spStatic> mObjects;
};
