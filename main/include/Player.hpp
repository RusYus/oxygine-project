#pragma once

#include "BasisObject.hpp"
#include "BasisEventProxy.hpp"
#include "Utils.hpp"
#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2Body.h"
#include "oxygine-framework.h"

using namespace oxygine;

constexpr const float SCALE = 100.0f;
constexpr const int PLAYER_MAX_SPEED = 900;
constexpr const int PLAYER_JUMP_SPEED = 700;

DECLARE_SMART(Player, spPlayer);
class Player: public Object, public BasisObject
{
public:
    Player();
    void Init(b2World*, spEventProxy);
    void Update(const UpdateState&);
    spActor GetView() const;
    void Move(Event*);
    void Jump(Event*);
    inline float GetX() const;
    inline float GetY() const;
    void SetNormal(const b2Vec2);
    void SetZeroNormal();
    b2Body* _body;

private:
    void _Init(b2World*);

private:
    spEventProxy _eventProxy;
    spActor _view;
    spSprite _box;
    b2Vec2 _direction;
    b2Vec2 _normal;
    bool _isJumping;
    const int _maxSpeed = PLAYER_MAX_SPEED;
    const int _jumpSpeed = PLAYER_JUMP_SPEED;

    std::pair<ObjectType, Player*> _bodyPair;
};
