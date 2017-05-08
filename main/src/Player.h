#pragma once

#include "PlayerEventProxy.h"
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
    void Init(spDemoLevel, spEventProxy);
    void Update(const UpdateState&);
    void Move(Event*);
    float GetX() const;
    float GetY() const;

private:
    void _Init(b2World*);
    b2Vec2 _Convert(const Vector2&);
    Vector2 _Convert(const b2Vec2&);
    inline bool _IsMoving() const;

private:
    spEventProxy _eventProxy;
    spDemoLevel _game;
    spActor _view;
    spSprite _box;
    b2Body* _body;
    Vector2 _direction;
    const int _maxSpeed = 900;
};
