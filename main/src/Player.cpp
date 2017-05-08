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

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(this);
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

    _direction = Vector2();
}

void Player::Jump(Event* /*aEvent*/)
{
//    PlayerJumpEvent* playerEvent = safeCast<PlayerJumpEvent*>(aEvent);
    if (!_isJumping)
    {
        _isJumping = true;
        _body->SetLinearVelocity(_Convert(Vector2(_direction.x, -_jumpSpeed)));
    }
}

void Player::Move(Event* aEvent)
{
    if (_body != nullptr)
    {
        PlayerMoveEvent* playerEvent = safeCast<PlayerMoveEvent*>(aEvent);

        Vector2 dir = _Convert(_body->GetLinearVelocity());

        if (playerEvent->_isMoving)
        {
            dir.x = playerEvent->_isMovingRight ? _maxSpeed : -_maxSpeed;
        }
        else
        {
            dir.x = 0;
        }

        std::cout << "Player:" << dir.x << ":" << dir.y << std::endl;
        _direction = dir;
        _body->SetLinearVelocity(_Convert(_direction));
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

inline bool Player::_IsMoving() const
{
    return _direction.x != .0f || _direction.y != .0f;
}

void Player::Update(const UpdateState& /*us*/)
{
    // Do I actually need this? Need to check out.
    if (_IsMoving())
    {
        std::cout << "Moving!" << _direction.x << ";" <<
                  _direction.y << std::endl;
        Vector2 newDir = _Convert(_body->GetLinearVelocity());
        newDir.x = _direction.x;
        _body->SetLinearVelocity(_Convert(newDir));
    }

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
