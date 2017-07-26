#include <iostream>

#include "SDL_keyboard.h"
#include "Player.hpp"
#include "DemoLevel.hpp"
#include "res.hpp"
#include "BasisEvents.hpp"


Player::Player()
    : mBodyPair(Service::ObjectType::Player, this)
    , mCollisionNormal(0, 0)
    , mIsButtonMoving(false)
    , mIsJumping(false)
{
}
void Player::InitBody(b2World* aWorld)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position = Service::Utils::Convert(mView->getPosition());

    _body = aWorld->CreateBody(&bodyDef);

    setUserData(_body);

    b2PolygonShape shape;
    float32 width = mBox->getWidth() / Service::Constants::SCALE / 2.0f;
    float32 height = mBox->getHeight() / Service::Constants::SCALE / 2.0f;
    shape.SetAsBox(width, height);

    b2Filter filter;
    filter.categoryBits = 0x0002;
    filter.maskBits = 0x0001;
    filter.groupIndex = 1;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 100.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter = filter;

    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(&mBodyPair);
}

spActor Player::GetView() const
{
    return mView;
}

void Player::Init(b2World* aWorld, spEventProxy aEventProxy)
{
    mView = new Actor;
    mView->setPosition(getStage()->getSize() / 2);

    mBox = new Sprite;
    mBox->setResAnim(res::ui.getResAnim("player"));
    mBox->attachTo(mView);
    mBox->setAnchor(Vector2(0.5f, 0.5f));

    InitBody(aWorld);

    mEventProxy = aEventProxy;

    mEventProxy->addEventListener(PlayerMoveEvent::EVENT, CLOSURE(this, &Player::ProcessMoveEvent));

    mEventProxy->addEventListener(PlayerJumpEvent::EVENT, CLOSURE(this, &Player::Jump));

    mDirection = _body->GetLinearVelocity();
}

void Player::Jump(Event* /*aEvent*/)
{
    if (!mIsJumping)
    {
        std::cout << "Jumping!" << std::endl;
        mIsJumping = true;
        _body->SetLinearVelocity(b2Vec2(mDirection.x, -mJumpSpeed / Service::Constants::SCALE));
    }
}

void Player::ProcessMoveEvent(Event* aEvent)
{
    PlayerMoveEvent* playerEvent = safeCast<PlayerMoveEvent*>(aEvent);
    if (!playerEvent->mIsMoving)
    {
        mIsButtonMoving = false;
        Stop();
    }
    else
    {
        mIsButtonMoving = true;
        Move(playerEvent->mIsMovingRight);
    }
}

void Player::Move(bool aIsMovingRight)
{
    if (_body != nullptr)
    {
        mDirection = _body->GetLinearVelocity();

//        std::cout << "Moving!" << "NORMALS:" << mNormal.x << ";" << mGroundNormal.x << std::endl;
        mDirection.x = aIsMovingRight ? mMaxSpeed : -mMaxSpeed;
        mDirection.x /= Service::Constants::SCALE;

        // Collision took place
        if ((aIsMovingRight && mCollisionNormal.x > 0) || (!aIsMovingRight && mCollisionNormal.x < 0))
        {
            std::cout << "Collision took place!" << std::endl;
            Stop();
        }

        _body->SetLinearVelocity(mDirection);
    }
}

inline void Player::Stop()
{
    mDirection.x = 0;
}

float Player::GetX() const
{
    return (mView.get() ? mView->getX() : .0f);
}

float Player::GetY() const
{
    return (mView.get() ? mView->getY() : .0f);
}

void Player::SetCollisionNormal(const b2Vec2 aNormal)
{
    std::cout << "Setting ground normal; old=" << mCollisionNormal.x << ":" << mCollisionNormal.y << ";new=" << aNormal.x << ":" << aNormal.y << std::endl;
    mCollisionNormal += aNormal;
    std::cout << "Ground:" << mCollisionNormal.x << ":" << mCollisionNormal.y << std::endl;
}

void Player::SetZeroCollisionNormal()
{
    std::cout << "Set Zero" << std::endl;
    mCollisionNormal.SetZero();
}

void Player::ProcessKeyboard()
{
    const Uint8* states = SDL_GetKeyboardState(nullptr);

    if (states[SDL_SCANCODE_LEFT])
    {
        Move(false);
    }
    else if (states[SDL_SCANCODE_RIGHT])
    {
        Move(true);
    }
    // Might move by onscreen buttons.
    else if (!mIsButtonMoving)
    {
        Stop();
    }

    if (states[SDL_SCANCODE_SPACE])
    {
        Jump(nullptr);
    }
}

void Player::Update(const UpdateState& /*us*/)
{
    ProcessKeyboard();

    mDirection.y = _body->GetLinearVelocity().y;

    // Reseting direction, if collision in place.
    if ((mDirection.x < 0 && mCollisionNormal.x < 0) || (mDirection.x > 0  && mCollisionNormal.x > 0))
    {
//        std::cout << "In Update: dir.x = 0" << std::endl;
        mDirection.x = 0;
    }

    _body->SetLinearVelocity(mDirection);

    // If player doesn't stand on something, he can't jump.
    if (mCollisionNormal.y == -1)
    {
        mIsJumping = false;
    }
    else
    {
        mIsJumping = true;
    }

    b2Vec2 b2pos = _body->GetPosition();
    Vector2 pos = Service::Utils::Convert(b2pos);
    CameraMovementEvent event(pos - mView->getPosition());
    mView->setPosition(pos);
    mEventProxy->dispatchEvent(&event);

//    std::cout << "Player: " << mGroundNormal.x << ":" << mGroundNormal.y << std::endl;
}