#pragma once

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
    Player() = default;
    void Init(b2World*, spEventProxy);
    void Update(const UpdateState&);
    void Move(const Vector2&);
    float GetX() const;
    float GetY() const;
    spActor GetView() const;

private:
    void _Init(b2World*);
    b2Vec2 _Convert(const Vector2&);

private:
    spEventProxy _eventProxy;
    spActor _view;
    spSprite _box;
    b2Body* _body;
    const int _speed = 9;
};
