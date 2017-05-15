#pragma once

#include "PlayerEventProxy.hpp"
#include "Utils.hpp"
#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2Body.h"
#include "oxygine-framework.h"

using namespace oxygine;

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

private:
    spEventProxy _eventProxy;
    spActor _view;
    spSprite _box;
    b2Body* _body;
    const int _speed = 9;
};
