#include "Player.h"
// TODO: Get rid of this mutual includeness
#include "Game.h"
#include "res.h"
#include "Joystick.h"

b2Vec2 Player::Convert(const Vector2& pos)
{
    return b2Vec2(pos.x / SCALE, pos.y / SCALE);
}

void Player::_init(b2World* world)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position = Convert(_view->getPosition());

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

void Player::_init()
{
    //initialize player's ship
    // TODO: Get size another way (mb change in Unit definition
    // to pass size?)
    _view->setPosition(_game->getSize() / 2);

    _box = new Sprite;
    _box->setResAnim(res::ui.getResAnim("player"));
    _box->attachTo(_view);
    _box->setAnchor(Vector2(0.5f, 0.5f));
}

void Player::_update(const UpdateState& us)
{
    Vector2 dir;

    if (_game->_move->getDirection(dir) && _body != nullptr)
    {
        _body->SetLinearVelocity(Convert(dir * _speed));
    }

    b2Vec2 b2pos = _body->GetPosition();
    Vector2 pos = Vector2(b2pos.x * SCALE, b2pos.y * SCALE);
    _view->setPosition(pos);
}

void Player::setPosition(const Vector2& pos)
{
    _view->setPosition(pos);
}

void Player::setRotation(float angle)
{
    _view->setRotation(angle);
}
