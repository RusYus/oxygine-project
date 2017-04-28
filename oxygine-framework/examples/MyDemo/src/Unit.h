#pragma once
#include "Box2D/Dynamics/b2Body.h"
#include "oxygine-framework.h"
using namespace oxygine;

class Game;
class Unit: public Object
{
public:
    Unit();

    void init(Game* game);

    void update(const UpdateState& us);

protected:
    virtual void _init() {}
    virtual void _update(const UpdateState& us) {}

    spActor _view;
    Game* _game;
};
