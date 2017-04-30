#pragma once

#include "DemoLevel.h"
#include "PlayerEventProxy.h"
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
    void Init(spDemoLevel, spEventProxy);
    void Update(const UpdateState&);
    void Move(const Vector2&);

private:
    void _Init(b2World*);
    b2Vec2 _Convert(const Vector2&);

private:
    spEventProxy _eventProxy;
    spActor _view;
    spDemoLevel _game;
    spSprite _box;
    b2Body* _body;
    const int _speed = 3;
};
