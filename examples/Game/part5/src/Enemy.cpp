#include "Box2D/Box2D.h"
#include "Enemy.h"
#include "res.h"

void Enemy::_init()
{
    //you could hit enemy 3 times
    _hp = 3;

    spSprite sprite = new Sprite;
    sprite->setResAnim(res::ui.getResAnim("asteroid"));
    sprite->attachTo(_view);
    sprite->setAnchor(Vector2(0.5f, 0.5f));

    //it random scale and rotation
    sprite->setRotation(scalar::randFloat(0, MATH_PI * 2));
    sprite->setScale(scalar::randFloat(0.5f, 1.0f));

    //it is rotating by tween with random speed
    float dest = MATH_PI * 2;
    if (rand() % 2 == 0)
        dest *= -1;
    dest += sprite->getRotation();
    sprite->addTween(Sprite::TweenRotation(dest), rand() % 12000 + 10000, -1);

    //----------------
    b2BodyDef bodyDef;
    bodyDef.position = b2Vec2(this->getPosition().x, this->getPosition().y);

    b2Body* body = world->CreateBody(&bodyDef);

    setUserData(body);

    b2CircleShape shape;
    shape.m_radius = std::static_cast<decltype(shape.m_radius)>(sprite->getHeight());

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    body->CreateFixture(&fixtureDef);
    body->SetUserData(this);

}

void Enemy::_update(const UpdateState& us)
{
    //nothing to do
}

void Enemy::explode()
{
    //hit by rocket
    _hp--;
    if (_hp == 0)
    {
        //dead, hide it with alpha tween
        _dead = true;
        _view->addTween(Actor::TweenAlpha(0), 300)->detachWhenDone();
    }
}
