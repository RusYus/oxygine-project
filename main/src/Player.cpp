#include "Player.h"
#include "Game.h"
#include "res.h"
#include "Joystick.h"

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
    fixtureDef.friction = 1.3f;

    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(this);
}

void Player::Init(Game* game)
{
    _game = game;

    _view = new Actor;
    _view->attachTo(game);
    _view->setPosition(_game->getSize() / 2);

    _box = new Sprite;
    _box->setResAnim(res::ui.getResAnim("player"));
    _box->attachTo(_view);
    _box->setAnchor(Vector2(0.5f, 0.5f));

    _Init(game->_world);
}

void Player::Update(const UpdateState& us)
{
    Vector2 dir;

    if (_game->_move->getDirection(dir) && _body != nullptr)
    {
        _body->SetLinearVelocity(_Convert(dir * _speed));
    }

    b2Vec2 b2pos = _body->GetPosition();
    Vector2 pos = Vector2(b2pos.x * SCALE, b2pos.y * SCALE);
    _view->setPosition(pos);
}

void Player::SetPosition(const Vector2& pos)
{
    _view->setPosition(pos);
}

void Player::SetRotation(float angle)
{
    _view->setRotation(angle);
}
