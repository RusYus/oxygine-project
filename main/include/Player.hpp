#pragma once

#include "BasisObject.hpp"
#include "BasisEventProxy.hpp"
#include "Utils.hpp"
#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2Body.h"
#include "oxygine-framework.h"

constexpr const float SCALE = 100.0f;
constexpr const int PLAYER_MAX_SPEED = 900;
constexpr const int PLAYER_JUMP_SPEED = 700;

DECLARE_SMART(Player, spPlayer);
class Player: public oxygine::Object, public BasisObject
{
public:
    Player();
    void Init(b2World*, spEventProxy);
    void Update(const oxygine::UpdateState&);
    oxygine::spActor GetView() const;
    void Move(oxygine::Event*);
    void Jump(oxygine::Event*);
    inline float GetX() const;
    inline float GetY() const;
    void SetNormal(const b2Vec2);
    void SetZeroNormal();
    b2Body* _body;

private:
    void _Init(b2World*);

private:
    spEventProxy mEventProxy;
    oxygine::spActor mView;
    oxygine::spSprite mBox;
    b2Vec2 mDirection;
    b2Vec2 mNormal;
    bool mIsJumping;
    const int mMaxSpeed = PLAYER_MAX_SPEED;
    const int mJumpSpeed = PLAYER_JUMP_SPEED;

    std::pair<Service::ObjectType, Player*> mBodyPair;
};
