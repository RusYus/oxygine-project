#include "Player.h"
#include "DemoLevel.h"
#include "res.h"
#include "Joystick.h"
#include "PlayerEvents.h"

b2Vec2 Player::_Convert(const Vector2& pos)
{
    return b2Vec2(pos.x / SCALE, pos.y / SCALE);
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

void Player::Init(spDemoLevel game, spEventProxy aEventProxy)
{
    _game = game;

    _view = new Actor;
    _view->attachTo(game);
    _view->setPosition(getStage()->getSize() / 2);

    _box = new Sprite;
    _box->setResAnim(res::ui.getResAnim("player"));
    _box->attachTo(_view);
    _box->setAnchor(Vector2(0.5f, 0.5f));

    _Init(game->_world);

    _eventProxy = aEventProxy;
}

void Player::Move(const Vector2& aDir)
{
    if (_body != nullptr)
    {
        _body->SetLinearVelocity(_Convert(aDir * _speed));
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

void Player::Update(const UpdateState& us)
{
    b2Vec2 b2pos = _body->GetPosition();
    Vector2 pos = Vector2(b2pos.x * SCALE, b2pos.y * SCALE);
    PlayerMovementEvent event(pos - _view->getPosition());
    _view->setPosition(pos);
    _eventProxy->dispatchEvent(&event);
}
