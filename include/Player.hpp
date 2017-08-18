#pragma once

#include "BasisEventProxy.hpp"
#include "CollisionInfo.hpp"
#include "IMovable.hpp"
#include "IDebugDrawable.hpp"
#include "Utils.hpp"

DECLARE_SMART(Player, spPlayer);
class Player: public oxygine::Actor, public virtual IMovable, public virtual IDebugDrawable
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
};
