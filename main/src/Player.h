#pragma once
#include "Box2D/Box2D.h"
#include "Unit.h"

constexpr const float SCALE = 100.0f;

DECLARE_SMART(Player, spPlayer);
class Player: public Unit
{
public:
    void _init(b2World*);
    void setPosition(const Vector2&);
    void setRotation(float);

protected:
    void _init();
    void _update(const UpdateState& us);

    spSprite _box;
    b2Body* _body;

private:
    b2Vec2 Convert(const Vector2&);

private:
    const int _speed = 3;
};
