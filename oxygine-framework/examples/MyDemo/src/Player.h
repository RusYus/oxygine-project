#pragma once
#include "Box2D/Box2D.h"
#include "Unit.h"

DECLARE_SMART(Player, spPlayer);
class Player: public Unit
{
public:
    void _init(b2World*);
    void SetMove(bool);
    void setPosition(const Vector2&);
    void setRotation(float);

protected:
    void _init();
    void _update(const UpdateState& us);

//    spSprite _engine;
    spSprite _ship;
    b2Body* _body;
private:
    bool _canMove;
};
