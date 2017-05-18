#include "Player.h"
#include "DemoLevel.h"
#include "res.h"
#include "Joystick.h"
#include "BasicEvents.h"

#include <iostream>

b2Vec2 Player::_Convert(const Vector2& pos)
{
    return b2Vec2(pos.x / SCALE, pos.y / SCALE);
}

Vector2 Player::_Convert(const b2Vec2& pos)
{
    return Vector2(pos.x * SCALE, pos.y * SCALE);
}

Player::Player()
    : _game(0)
    , _bodyPair(ObjectType::Player, this)
    , _normal(0, 0)
{
}

void Player::_Init(b2World* world)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position = _Convert(_view->getPosition());

    _body = world->CreateBody(&bodyDef);

    setUserData(_body);

    b2PolygonShape shape;
    shape.SetAsBox(_box->getWidth() / SCALE / 2.0f, _box->getHeight() / SCALE / 2.0f);

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
    _body->SetUserData(&_bodyPair);
}

void Player::Init(spDemoLevel aGame, spEventProxy aEventProxy)
{
    _game = aGame;

    _view = new Actor;
    _view->attachTo(_game);
    _view->setPosition(getStage()->getSize() / 2);

    _box = new Sprite;
    _box->setResAnim(res::ui.getResAnim("player"));
    _box->attachTo(_view);
    _box->setAnchor(Vector2(0.5f, 0.5f));

    _Init(_game->_world);

    _eventProxy = aEventProxy;

    _eventProxy->addEventListener(PlayerMoveEvent::EVENT, CLOSURE(this, &Player::Move));

    _eventProxy->addEventListener(PlayerJumpEvent::EVENT, CLOSURE(this, &Player::Jump));

    _direction = _body->GetLinearVelocity();
}

void Player::Jump(Event* /*aEvent*/)
{
    if (!_isJumping)
    {
        _isJumping = true;
        _body->SetLinearVelocity(b2Vec2(_direction.x, -_jumpSpeed / SCALE));

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

        _direction = _body->GetLinearVelocity();

        if (playerEvent->_isMoving)
        {
            _direction.x = playerEvent->_isMovingRight ? _maxSpeed : -_maxSpeed;
            _direction.x /= SCALE;

            // Moving opposing direction of collision
            // If collision more than one, need more normals
            // Therefore, this is not gonna work
            if ((playerEvent->_isMovingRight && _normal.x < 0)
                || (!playerEvent->_isMovingRight && _normal.x > 0))
            {
                SetZeroNormal();
            }
        }

        // Collision took place, or move button released.
        if (!playerEvent->_isMoving
            || (playerEvent->_isMovingRight && _normal.x > 0)
            || (!playerEvent->_isMovingRight && _normal.x < 0))
        {
            _direction.x = 0;
        }

        _body->SetLinearVelocity(_direction);
    }
}

float Player::GetX() const
{
    return (_view.get() ? _view->getX() : .0f);
}

float Player::GetY() const
{
    return (_view.get() ? _view->getY() : .0f);
}

void Player::SetNormal(const b2Vec2 aNormal)
{
    _normal = aNormal;
}

void Player::SetZeroNormal()
{
    _normal.SetZero();
}

void Player::Update(const UpdateState& /*us*/)
{
//    std::cout << _direction.x << " : " << _direction.y << std::endl;
    _direction.y = _body->GetLinearVelocity().y;

    // Reseting direction, if collision in place.
    if (_normal.x != 0)
    {
        _direction.x = 0;
    }
    _body->SetLinearVelocity(_direction);

    if (_body->GetLinearVelocity().y == .0f)
    {
        _isJumping = false;
    }

    b2Vec2 b2pos = _body->GetPosition();
    Vector2 pos = _Convert(b2pos);
    CameraMovementEvent event(pos - _view->getPosition());
    _view->setPosition(pos);
    _eventProxy->dispatchEvent(&event);
}
