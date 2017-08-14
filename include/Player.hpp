#pragma once

#include "BasisObject.hpp"
#include "BasisEventProxy.hpp"
#include "CollisionInfo.hpp"
#include "IMovable.hpp"
#include "Utils.hpp"
#include "Box2D/Box2D.h"
#include "Box2D/Dynamics/b2Body.h"
#include "oxygine-framework.h"

DECLARE_SMART(Player, spPlayer);
class Player: public oxygine::Actor, public IMovable
{
public:
    Player();
    ~Player();
    void SetPosition() override;

    void Init(spEventProxy);
    void Update(const oxygine::UpdateState&);
    oxygine::spActor GetView() const;
    void ProcessMoveEvent(oxygine::Event*);
    void Jump(oxygine::Event*);

    void doRender(const oxygine::RenderState&);

private:
    void Move(bool /*aIsMovingRight*/);
    inline void Stop();
    void ProcessKeyboard();

private:
    spEventProxy m_EventProxy;
    bool m_IsJumping;
    bool m_IsButtonMoving;
    const int m_MaxSpeed = Service::Constants::PLAYER_MAX_SPEED;
    const int m_JumpSpeed = Service::Constants::PLAYER_JUMP_SPEED;

    std::pair<Service::ObjectType, Player*> mBodyPair;
};
