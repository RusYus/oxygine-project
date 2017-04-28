#pragma once

#include "DemoLevel.h"
#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2Body.h"
#include "oxygine-framework.h"

using namespace oxygine;

constexpr const float SCALE = 100.0f;

DECLARE_SMART(Player, spPlayer);
class Player: public Object
{
public:
    Player();
    void Init(DemoLevel*, spEventProxy);
    void Update(const UpdateState&);
    void SetPosition(const Vector2&);
    void SetRotation(float);

private:
    void _Init(b2World*);
    b2Vec2 _Convert(const Vector2&);

private:
    spEventProxy _eventProxy;
    spActor _view;
    DemoLevel* _game;
    spSprite _box;
    b2Body* _body;
    const int _speed = 3;
};
