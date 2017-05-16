#include "Player.hpp"
#include "DemoLevel.hpp"
#include "res.hpp"
#include "Joystick.hpp"
#include "PlayerEvents.hpp"

void Player::_Init(b2World* aWorld)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.fixedRotation = true;
    bodyDef.position = Service::Utils::Convert(_view->getPosition());

    _body = aWorld->CreateBody(&bodyDef);

    setUserData(_body);

    b2PolygonShape shape;
    float32 width = _box->getWidth() / Service::Constants::SCALE / 2.0f;
    float32 height = _box->getHeight() / Service::Constants::SCALE / 2.0f;
    shape.SetAsBox(width, height);

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(this);
}

spActor Player::GetView() const
{
    return _view;
}

void Player::Init(b2World* aWorld, spEventProxy aEventProxy)
{
    _view = new Actor;
    _view->setPosition(getStage()->getSize() / 2);

    _box = new Sprite;
    _box->setResAnim(res::ui.getResAnim("player"));
    _box->attachTo(_view);
    _box->setAnchor(Vector2(0.5f, 0.5f));

    _Init(aWorld);

    _eventProxy = aEventProxy;
}

void Player::Move(const Vector2& aDir)
{
    if (_body != nullptr)
    {
        _body->SetLinearVelocity(Service::Utils::Convert(aDir * _speed));
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
    Vector2 pos = Service::Utils::Convert(b2pos);
    PlayerMovementEvent event(pos - _view->getPosition());
    _view->setPosition(pos);
    _eventProxy->dispatchEvent(&event);
}
