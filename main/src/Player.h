#pragma once
#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2Body.h"
#include "oxygine-framework.h"

using namespace oxygine;

constexpr const float SCALE = 100.0f;

class Game;
class Player: public Object
{
public:
    Player();
    void Init(Game* game);
    void Update(const UpdateState& us);
    void SetPosition(const Vector2&);
    void SetRotation(float);

private:
    void _Init(b2World*);
    b2Vec2 _Convert(const Vector2&);

private:
    spActor _view;
    Game* _game;
    spSprite _box;
    b2Body* _body;
    const int _speed = 3;
};

DECLARE_SMART(Player, spPlayer);
