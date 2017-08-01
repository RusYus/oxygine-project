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

spActor Player::GetView() const
{
    return mView;
}

void Player::Init(spEventProxy aEventProxy)
{
    mView = new Actor;
    oxygine::Vector2 pos = getStage()->getSize() / 2;



    mBox = new Sprite;
    mBox->setResAnim(res::ui.getResAnim("player"));
    mBox->attachTo(mView);
//    mBox->setAnchor(Vector2(0.5f, 0.5f));
    mView->setPosition(100, 100);
    mView->setSize(mBox->getSize());

    mEventProxy = aEventProxy;

    mEventProxy->addEventListener(PlayerMoveEvent::EVENT, CLOSURE(this, &Player::ProcessMoveEvent));

    mEventProxy->addEventListener(PlayerJumpEvent::EVENT, CLOSURE(this, &Player::Jump));

    mDirection = oxygine::Vector2();

    int actualIntervalsNumber = static_cast<int>(std::ceil(GetWidth() / RAYCAST_INTERVAL));
    float actualIntervalLength = GetWidth() / actualIntervalsNumber;
    actualIntervalsNumber--;
    for (int i = 0; i < actualIntervalsNumber + 2; ++i)
    {
        // Bottom
        mRays.emplace_back(Ray(oxygine::Vector2(GetX() + i * actualIntervalLength, GetY() + GetHeight()), oxygine::Vector2(GetX() + i * actualIntervalLength, GetY() + GetHeight())));
        // Top
//        mRays.emplace_back(Ray(oxygine::Vector2(GetX() + i * actualIntervalLength, GetY()), oxygine::Vector2(GetX() + i * actualIntervalLength, GetY())));
    }

//    actualIntervalsNumber = static_cast<int>(std::ceil(GetHeight() / RAYCAST_INTERVAL));
//    actualIntervalLength = GetHeight() / actualIntervalsNumber;
//    actualIntervalsNumber--;
//    for (int i = 0; i < actualIntervalsNumber + 2; ++i)
//    {
//        // Right
//        mRays.emplace_back(Ray(oxygine::Vector2(GetX() + GetWidth(), GetY() + i * actualIntervalLength), oxygine::Vector2(GetX() + GetWidth(), GetY() + i * actualIntervalLength)));
//        // Left
//        mRays.emplace_back(Ray(oxygine::Vector2(GetX(), GetY() + i * actualIntervalLength), oxygine::Vector2(GetX(), GetY() + i * actualIntervalLength)));
//    }
}

void Player::Jump(Event* /*aEvent*/)
{
    if (!mIsJumping)
    {
        std::cout << "Jumping!------------------------------------------------------------" << std::endl;
        mIsJumping = true;
        mDirection.y -= mJumpSpeed;
//        _body->SetLinearVelocity(b2Vec2(mDirection.x, -mJumpSpeed / Service::Constants::SCALE));
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
//    if (_body != nullptr)
//    {
//        mDirection = _body->GetLinearVelocity();

////        std::cout << "Moving!" << "NORMALS:" << mNormal.x << ";" << mGroundNormal.x << std::endl;
        mDirection.x = aIsMovingRight ? mMaxSpeed : -mMaxSpeed;
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
    mDirection.x = 0;
}

void Player::SetY(float newPosY)
{
    mDirection.y = newPosY;
}

float Player::GetX() const
{
    return (mView.get() ? mView->getX() : .0f);
}

float Player::GetY() const
{
    return (mView.get() ? mView->getY() : .0f);
}

float Player::GetWidth() const
{
    return (mView.get() ? mView->getWidth() : .0f);
}

float Player::GetHeight() const
{
    return (mView.get() ? mView->getHeight() : .0f);
}

oxygine::Vector2 Player::GetRayOriginal() const
{
    return mRays.back().Original;
}

oxygine::Vector2 Player::GetRayDestination() const
{
    return mRays.back().Destination;
}
std::vector<Ray>& Player::GetRays()
{
    return mRays;
}

void Player::SetCollisionNormal(const oxygine::Vector2 aNormal)
{
//    std::cout << "Setting ground normal; old=" << mCollisionNormal.x << ":" << mCollisionNormal.y << ";new=" << aNormal.x << ":" << aNormal.y << std::endl;
//    mCollisionNormal += aNormal;
//    std::cout << "Ground:" << mCollisionNormal.x << ":" << mCollisionNormal.y << std::endl;

    mCollisionNormal += aNormal;
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

void Player::SetPosition()
{
    Vector2 newPos = mView->getPosition() + mDirection;

    std::cout << "Pos: " << newPos.x << " : " << newPos.y << std::endl;
    mView->setPosition(newPos);

    for(int i = 0; i < mRays.size(); ++i)
    {
        mRays.at(i).Original = oxygine::Vector2(GetX() + i * RAYCAST_INTERVAL, GetY() + GetHeight());
    }
}

oxygine::Vector2 Player::GetPosition() const
{
    return mView->getPosition();
}

oxygine::Vector2 Player::GetDirection() const
{
    return mDirection;
}

Service::Normal2 Player::GetCollisionNormal() const
{
    return mCollisionNormal;
}

void Player::Update(const UpdateState& us)
{
    ProcessKeyboard();

//    mDirection.y = _body->GetLinearVelocity().y;

    // Reseting direction, if collision in place.
    if ((mDirection.x < 0 && mCollisionNormal.x < 0) || (mDirection.x > 0  && mCollisionNormal.x > 0))
    {
//        std::cout << "In Update: dir.x = 0" << std::endl;
        mDirection.x = 0;
    }

//    _body->SetLinearVelocity(mDirection);

    mDirection.y += us.dt / GRAVITY;

    for (auto& ray : mRays)
    {
        ray.Destination = ray.Original + mDirection;
    }

    std::cout << "Dt:" << us.dt << std::endl;


    // If player doesn't stand on something, he can't jump.
    if (mCollisionNormal.y == -1)
    {
        mIsJumping = false;
    }
    else
    {
        mIsJumping = true;
    }

    std::cout << "Player:"
              << mCollisionNormal.x << ":" << mCollisionNormal.y << std::endl;
//              << mView->getX() << ":" << mView->getY() << std::endl;

//    b2Vec2 b2pos = _body->GetPosition();
//    Vector2 pos = Service::Utils::Convert(b2pos);
//    CameraMovementEvent event(pos - mView->getPosition());
//    mView->setPosition(pos);
//    mEventProxy->dispatchEvent(&event);

//    std::cout << "Player: " << mGroundNormal.x << ":" << mGroundNormal.y << std::endl;
}
