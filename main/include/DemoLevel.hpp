#pragma once

#include "oxygine-framework.h"
#include "res.hpp"
#include "core/file.h"
#include "res/SingleResAnim.h"

#include "BasisObject.hpp"
#include "JsonImporter.hpp"
#include "Box2D/Box2D.h"
#include "Utils.hpp"
#include <forward_list>

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
    std::pair<Service::ObjectType, Circle*> _bodyPair;
};

class Square : public Sprite, public BasisObject
{
public:
    Square(b2World*, const Vector2&, float);
    void Update();
    bool IsAlive = true;

public:
    b2Body* _body;
    std::pair<Service::ObjectType, Square*> _bodyPair;
};

class Static : public Box9Sprite, public BasisObject
{
public:
    Static(b2World*, const RectF&);

public:
    std::pair<Service::ObjectType, Static*> _bodyPair;
};

class DemoLevel: public Actor
{
public:
    DemoLevel();

    void Init(b2World*, Service::JsonImporter&);
    void click(Event*);
    void showHideDebug(Event*);
    void doRender(const RenderState&);

private:
    friend class Player;
    void doUpdate(const UpdateState&);
    void drawLayer(int, int, int, int);
    void CreateTileSetTexture(Image&);

    b2World* _world;
    spNativeTexture _texture;
    int _mapHeight = -1;
    int _mapWidth = -1;
    int _tileHeight = -1;
    int _tileWidth = -1;
    std::vector<int> _tilePositions;
    int cols;
    int rows;
    std::forward_list<spCircle> _circles;
    std::forward_list<spSquare> _squares;
};
