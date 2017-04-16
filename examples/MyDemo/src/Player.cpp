#include "Player.h"
#include "Game.h"
#include "res.h"
#include "Joystick.h"
#include <iostream>

const float SCALE = 100.0f;
b2Vec2 convert2(const Vector2& pos)
{
    return b2Vec2(pos.x / SCALE, pos.y / SCALE);
}

void Player::SetMove(bool aMove)
{
    _canMove = aMove;
}

void Player::_init(b2World* world)
{
    _canMove = true;
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = convert2(_view->getPosition());

    _body = world->CreateBody(&bodyDef);

    setUserData(_body);

//    b2PolygonShape shape;
//    b2Vec2 sz = convert2(_view->getSize() / 2);
//    shape.SetAsBox(sz.x, sz.y);
    b2PolygonShape shape;
    shape.SetAsBox(100/SCALE/2.0f, 100/SCALE/2.0f);
    std::cout << "RADIUS!!!!!" << _ship->getHeight() << std::endl;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
//    fixtureDef.isSensor= true;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

//    _body->SetBullet(true);

    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(this);
}

void Player::_init()
{
    _canMove = true;
    //initialize player's ship
    _view->setPosition(_game->getSize() / 2);

    _ship = new Sprite;
    _ship->setResAnim(res::ui.getResAnim("player"));
    _ship->attachTo(_view);
    _ship->setAnchor(Vector2(0.5f, 0.5f));

//    _engine = new Sprite;
//    _engine->setResAnim(res::ui.getResAnim("shipengine"));
//    _engine->attachTo(_ship);
//    //animate engine's fire
//    _engine->addTween(Sprite::TweenColor(Color::Red), 500, -1, true);
//    //by default it is hidden
//    //and would be visible only if ship moves
//    _engine->setVisible(false);
}

void Player::_update(const UpdateState& us)
{
//    _engine->setVisible(false);

//    const b2Vec2& pos = _body->GetPosition();
//    setPosition(Vector2(pos.x * 100, pos.y * 100));
//    setRotation(_body->GetAngle());


        Vector2 dir;
    if (_game->_move->getDirection(dir))
    {
        if (_canMove)
        {
            //update player position according to delta time and finger direction from virtual joystick
            Vector2 pos = _view->getPosition();
            pos = pos + dir * (us.dt / 1000.0f) * 5;
            _view->setPosition(pos);

            //rotate it
            float angle = atan2f(dir.y, dir.x);
            _view->setRotation(angle);

            //if player moves show engine's fire
//            _engine->setVisible(true);
            if (_body != nullptr)
            {
//                _body->SetTransform(convert2(pos), static_cast<float32>(angle));
                _body->SetLinearVelocity(convert2(dir));
            }
        }
    }
}

void Player::setPosition(const Vector2& pos)
{
    _view->setPosition(pos);
}

void Player::setRotation(float angle)
{
    _view->setRotation(angle);
}
