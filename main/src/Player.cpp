#include "Player.hpp"
#include "DemoLevel.hpp"
#include "res.hpp"
#include "BasisEvents.hpp"

#include <iostream>

Player::Player()
    : mBodyPair(Service::ObjectType::Player, this)
    , mNormal(0, 0)
{
}
void Player::_Init(b2World* aWorld)
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
    fixtureDef.density = 1.0f;
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

    _Init(aWorld);

    mEventProxy = aEventProxy;

    mEventProxy->addEventListener(PlayerMoveEvent::EVENT, CLOSURE(this, &Player::Move));

    mEventProxy->addEventListener(PlayerJumpEvent::EVENT, CLOSURE(this, &Player::Jump));

    mDirection = _body->GetLinearVelocity();
}

void Player::Jump(Event* /*aEvent*/)
{
    if (!mIsJumping)
    {
        mIsJumping = true;
        _body->SetLinearVelocity(b2Vec2(mDirection.x, -mJumpSpeed / SCALE));

        // This most likely not gonna work in bigger objects,
        // when jumping height less than their heights
        // So collision is still intact
        // Need to check this, but far later.
        SetZeroNormal();
    }
}

void Player::Move(Event* aEvent)
{
    if (_body != nullptr)
    {
        PlayerMoveEvent* playerEvent = safeCast<PlayerMoveEvent*>(aEvent);

        mDirection = _body->GetLinearVelocity();

        if (playerEvent->mIsMoving)
        {
            mDirection.x = playerEvent->mIsMovingRight ? mMaxSpeed : -mMaxSpeed;
            mDirection.x /= SCALE;

            // Moving opposing direction of collision
            // If collision more than one, need more normals
            // Therefore, this is not gonna work
            if ((playerEvent->mIsMovingRight && mNormal.x < 0)
                || (!playerEvent->mIsMovingRight && mNormal.x > 0))
            {
                SetZeroNormal();
            }
        }

        // Collision took place, or move button released.
        if (!playerEvent->mIsMoving
            || (playerEvent->mIsMovingRight && mNormal.x > 0)
            || (!playerEvent->mIsMovingRight && mNormal.x < 0))
        {
            mDirection.x = 0;
        }

        _body->SetLinearVelocity(mDirection);
    }
}

float Player::GetX() const
{
    return (mView.get() ? mView->getX() : .0f);
}

float Player::GetY() const
{
    return (mView.get() ? mView->getY() : .0f);
}

void Player::SetNormal(const b2Vec2 aNormal)
{
    mNormal = aNormal;
}

void Player::SetZeroNormal()
{
    mNormal.SetZero();
}

void Player::Update(const UpdateState& /*us*/)
{
//    std::cout << _direction.x << " : " << _direction.y << std::endl;
    mDirection.y = _body->GetLinearVelocity().y;

    // Reseting direction, if collision in place.
    if (mNormal.x != 0)
    {
        mDirection.x = 0;
    }
    _body->SetLinearVelocity(mDirection);

    if (_body->GetLinearVelocity().y == .0f)
    {
        mIsJumping = false;
    }

    b2Vec2 b2pos = _body->GetPosition();
    Vector2 pos = Service::Utils::Convert(b2pos);
    CameraMovementEvent event(pos - mView->getPosition());
    mView->setPosition(pos);
    mEventProxy->dispatchEvent(&event);
}
