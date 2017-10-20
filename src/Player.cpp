#include <iostream>

#include "Player.hpp"
#include "DemoLevel.hpp"
#include "res.hpp"
#include "BasisEvents.hpp"


Player::Player(spEventProxy aEventProxy, const std::shared_ptr<ICollisionManager>& a_Manager)
    : IMovable(a_Manager)
    , m_IsButtonMoving(false)
    , m_IsJumping(false)
{
    std::cout << "Player ID:" << GetId() << std::endl;
//    m_View = new Actor;
//    Service::Vector2L pos = getStage()->getSize() / 2;

    m_Box->setResAnim(res::ui.getResAnim("player"));
//    mBox->setAnchor(Vector2(0.5f, 0.5f));
    m_Position.set(9'000, 5'000);
    m_View->setPosition(Service::Convert(m_Position));
    m_View->setSize(m_Box->getSize());

    addChild(m_View);

    m_EventProxy = aEventProxy;
    m_EventProxy->addEventListener(PlayerMoveEvent::EVENT, CLOSURE(this, &Player::ProcessMoveEvent));
    m_EventProxy->addEventListener(PlayerJumpEvent::EVENT, CLOSURE(this, &Player::ProcessJumpEvent));

    m_Direction.setZero();

    SetRays();
}

Player::~Player()
{
}

spActor Player::GetView() const
{
    return m_View;
}

bool Player::IsJumping() const
{
    return m_IsJumping;
}

void Player::ProcessJumpEvent(oxygine::Event* /*aEvent*/)
{
    Jump();
}

void Player::Jump()
{
    //    std::cout << m_CollisionNormal.y << std::endl;
    if (!m_IsJumping)
    {
        std::cout << "Jumping!------------------------------------------------------------" << std::endl;
        m_IsJumping = true;
        m_Direction.y = -m_JumpSpeed;
    }
}

void Player::ProcessMoveEvent(Event* aEvent)
{
    PlayerMoveEvent* playerEvent = oxygine::safeCast<PlayerMoveEvent*>(aEvent);
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
        Jump();
    }
}

void Player::AddDirection(const Service::Vector2L& a_Direction)
{
    m_Direction += a_Direction;
//    std::cout << "Added direction, new:" << m_Direction.x << ":" << m_Direction.y << std::endl;
}

void Player::SetPosition()
{
    // Reseting direction, if collision in place.
    if ((m_Direction.x < 0 && m_CollisionNormal.x < 0) || (m_Direction.x > 0  && m_CollisionNormal.x > 0))
    {
        m_Direction.x = 0;
        std::cout << "Reseting direction x!!!" << std::endl;
    }

//    std::cout << "In Set. Player:" << m_Direction.x << ":" << m_Direction.y << " ; Pos:" << m_Position.x << ":" << m_Position.y << std::endl;
//    std::cout << "IsJumping:" << m_IsJumping << std::endl;

    IMovable::SetPosition();

    if (m_DebugDraw)
    {
        UpdateRays();
    }

    if (m_CollisionNormal.y == -1 && m_IsJumping)
    {
        m_IsJumping = false;
    }

//    std::cout << "End of step, collision normal:" << m_CollisionNormal.x << ":" << m_CollisionNormal.y
//              << "; IsJumping:" << m_IsJumping
//              << "; Direction:" << m_Direction.x << ":" << m_Direction.y << std::endl;

//        std::cout << "Player:"
////                  << m_Direction.x << ":" << m_Direction.y << "  |  "
//                     << GetX() << ":" << GetY() << "  |  "
////                  << m_CollisionNormal.x << ":" << m_CollisionNormal.y
//                  << std::endl;

}

void Player::Update(const UpdateState& /*aUpdateState*/)
{
    ProcessKeyboard();

//    // Reseting direction, if collision in place.
//    if ((m_Direction.x < 0 && m_CollisionNormal.x < 0) || (m_Direction.x > 0  && m_CollisionNormal.x > 0))
//    {
//        m_Direction.x = 0;
//        std::cout << "Reseting direction x!!!" << std::endl;
//    }

    m_Direction.y += Service::Constants::GRAVITY;

//    std::cout << "Player:" << m_Direction.x << ":" << m_Direction.y << " ; Pos:" << m_Position.x << ":" << m_Position.y << std::endl;

//    std::cout << "Update:" << m_Direction.x << ":" << m_Direction.y << std::endl;

//    std::cout << "Player:-----------------" << std::endl;
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
