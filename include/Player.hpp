#pragma once

#include "BasisObject.hpp"
#include "BasisEventProxy.hpp"
#include "Utils.hpp"
#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2Body.h"
#include "oxygine-framework.h"

constexpr const int PLAYER_MAX_SPEED = 6;
constexpr const int PLAYER_JUMP_SPEED = 5;
constexpr const float GRAVITY = 50;

DECLARE_SMART(Player, spPlayer);
class Player: public oxygine::Object, public Basis::BasisObject
{
public:
    Player();
    void Init(spEventProxy);
    void Update(const oxygine::UpdateState&);
    oxygine::spActor GetView() const;
    void ProcessMoveEvent(oxygine::Event*);
    void Jump(oxygine::Event*);
     void SetY(float);
     float GetX() const;
     float GetY() const;
     float GetWidth() const;
     float GetHeight() const;
     void SetPosition();
     oxygine::Vector2 GetPosition() const;
     oxygine::Vector2 GetDirection() const;
    void SetCollisionNormal(const oxygine::Vector2);
    void SetZeroCollisionNormal();
    // TODO : Move to private.

private:
    void Move(bool /*aIsMovingRight*/);
    inline void Stop();
    void ProcessKeyboard();

private:
    spEventProxy mEventProxy;
    oxygine::spActor mView;
    oxygine::spSprite mBox;
    oxygine::Vector2 mDirection;
    Service::Normal2 mCollisionNormal;
    oxygine::Matrix mTransform;
    bool mIsJumping;
    bool mIsButtonMoving;
    // TODO : in box2d terms (float32?).
    // as now every time it's being divided by scale.
    const int mMaxSpeed = PLAYER_MAX_SPEED;
    const int mJumpSpeed = PLAYER_JUMP_SPEED;

    std::pair<Service::ObjectType, Player*> mBodyPair;
};
