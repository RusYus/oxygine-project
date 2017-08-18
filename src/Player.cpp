#include <iostream>

#include "Player.hpp"
#include "DemoLevel.hpp"
#include "res.hpp"
#include "BasisEvents.hpp"


Player::Player()
    : m_IsButtonMoving(false)
    , m_IsJumping(false)
{
}

Player::~Player()
{
}

spActor Player::GetView() const
{
    return m_View;
}

void Player::Init(spEventProxy aEventProxy)
{
//    m_View = new Actor;
    oxygine::Vector2 pos = getStage()->getSize() / 2;

    m_Box->setResAnim(res::ui.getResAnim("player"));
//    mBox->setAnchor(Vector2(0.5f, 0.5f));
    m_View->setPosition(600, 100);
    m_View->setSize(m_Box->getSize());

    addChild(m_View);

    m_EventProxy = aEventProxy;

    m_EventProxy->addEventListener(PlayerMoveEvent::EVENT, CLOSURE(this, &Player::ProcessMoveEvent));

    m_EventProxy->addEventListener(PlayerJumpEvent::EVENT, CLOSURE(this, &Player::Jump));

    m_Direction = oxygine::Vector2();

    SetRays();
}

void Player::Jump(Event* /*aEvent*/)
{
//    std::cout << m_CollisionNormal.y << std::endl;
    if (!m_IsJumping)
    {
        std::cout << "Jumping!------------------------------------------------------------" << std::endl;
        m_IsJumping = true;
        m_Direction.y -= m_JumpSpeed;
    }
}

void Player::ProcessMoveEvent(Event* aEvent)
{
    PlayerMoveEvent* playerEvent = safeCast<PlayerMoveEvent*>(aEvent);
    if (!playerEvent->mIsMoving)
    {
        m_IsButtonMoving = false;
        Stop();
    }
    else
    {
        m_IsButtonMoving = true;
        Move(playerEvent->mIsMovingRight);
    }
}

void Player::Move(bool aIsMovingRight)
{
//    if (_body != nullptr)
//    {
//        mDirection = _body->GetLinearVelocity();

////        std::cout << "Moving!" << "NORMALS:" << mNormal.x << ";" << mGroundNormal.x << std::endl;
        m_Direction.x = aIsMovingRight ? m_MaxSpeed : -m_MaxSpeed;
//        mDirection.x /= Service::Constants::SCALE;

//        // Collision took place
//        if ((aIsMovingRight && mCollisionNormal.x > 0) || (!aIsMovingRight && mCollisionNormal.x < 0))
//        {
//            std::cout << "Collision took place!" << std::endl;
//            Stop();
//        }

//        _body->SetLinearVelocity(mDirection);
//    }
}

inline void Player::Stop()
{
    m_Direction.x = 0;
}

void Player::ProcessKeyboard()
{
    if (oxygine::key::isPressed(SDL_SCANCODE_LEFT))
    {
        Move(false);
    }
    else if (oxygine::key::isPressed(SDL_SCANCODE_RIGHT))
    {
        Move(true);
    }
    // Might move by onscreen buttons.
    else if (!m_IsButtonMoving)
    {
        Stop();
    }

    if (oxygine::key::wasPressed(SDL_SCANCODE_SPACE))
    {
        Jump(nullptr);
    }
}

void Player::SetPosition()
{
    IMovable::SetPosition();

    if (m_DebugDraw)
    {
        UpdateRays();
    }

    // If player doesn't stand on something, he can't jump.
    if (m_CollisionNormal.y == -1)
    {
        m_IsJumping = false;
    }
    else
    {
        m_IsJumping = true;
    }

//        std::cout << "Player:"
////                  << m_Direction.x << ":" << m_Direction.y << "  |  "
//                     << GetX() << ":" << GetY() << "  |  "
////                  << m_CollisionNormal.x << ":" << m_CollisionNormal.y
//                  << std::endl;

}

void Player::Update(const UpdateState& us)
{
    ProcessKeyboard();

    // Reseting direction, if collision in place.
    if ((m_Direction.x < 0 && m_CollisionNormal.x < 0) || (m_Direction.x > 0  && m_CollisionNormal.x > 0))
    {
        m_Direction.x = 0;
    }

    m_Direction.y += us.dt / static_cast<float>(Service::Constants::GRAVITY);

//    std::cout << "Update:" << m_Direction.x << ":" << m_Direction.y << std::endl;

    UpdateRays();

//    std::cout << "Dt:" << us.dt << std::endl;

//    std::cout << "Player:"
//              << mCollisionNormal.x << ":" << mCollisionNormal.y << std::endl;
//              << m_View->getX() << ":" << m_View->getY() << std::endl;

//    b2Vec2 b2pos = _body->GetPosition();
//    Vector2 pos = Service::Utils::Convert(b2pos);
//    CameraMovementEvent event(pos - m_View->getPosition());
//    m_View->setPosition(pos);
//    mEventProxy->dispatchEvent(&event);

//    std::cout << "Player: " << mGroundNormal.x << ":" << mGroundNormal.y << std::endl;
}

void Player::doRender(const oxygine::RenderState& a_State)
{
    DrawCollisionRays(m_Rays, a_State.transform);
}
