#include "DemoLevel.hpp"
#include "Joystick.hpp"
#include "res.hpp"
#include <iostream>

Circle::Circle(b2World* world, const Vector2& pos, float scale = 1)
    : _bodyPair(ObjectType::DynamicBody, this)
{
    setResAnim(res::ui.getResAnim("circle"));
    setAnchor(Vector2(0.5f, 0.5f));
    setTouchChildrenEnabled(false);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = Service::Utils::Convert(pos);

    _body = world->CreateBody(&bodyDef);

    setUserData(_body);

    setScale(scale);

    b2CircleShape shape;
    shape.m_radius = getWidth() / Service::Constants::SCALE / 2 * scale;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(&_bodyPair);
}

void Circle::Update()
{
    const b2Vec2& pos = _body->GetPosition();
    setPosition(Vector2(pos.x * 100, pos.y * 100));
    setRotation(_body->GetAngle());
}

Square::Square(b2World* world, const Vector2& pos, float scale = 1)
    : _bodyPair(ObjectType::DynamicBody, this)
{
    setResAnim(res::ui.getResAnim("square"));
    setAnchor(Vector2(0.5f, 0.5f));
    setTouchChildrenEnabled(false);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = Service::Utils::Convert(pos);
    bodyDef.fixedRotation = true;

    _body = world->CreateBody(&bodyDef);

    setUserData(_body);

    setScale(scale);

    b2PolygonShape shape;
    shape.SetAsBox(getWidth() / Service::Constants::SCALE / 2.0f, getHeight() / Service::Constants::SCALE / 2.0f);

    b2Filter filter;
    filter.categoryBits = 0x0003;
    filter.maskBits = 0x0001;
    filter.groupIndex = 2;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 100.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter = filter;

    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(&_bodyPair);
}

void Square::Update()
{
    const b2Vec2& pos = _body->GetPosition();
    setPosition(Vector2(pos.x * 100, pos.y * 100));
    setRotation(_body->GetAngle());
}

Static::Static(b2World* world, const RectF& rc)
    : _bodyPair(ObjectType::Ground, this)
{
    setResAnim(res::ui.getResAnim("pen"));
    setSize(rc.getSize());
    setPosition(rc.getLeftTop());
    setAnchor(Vector2(0.5f, 0.5f));

    b2BodyDef groundBodyDef;
    groundBodyDef.position = Service::Utils::Convert(getPosition());

    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    b2Vec2 sz = Service::Utils::Convert(getSize() / 2);
    groundBox.SetAsBox(sz.x, sz.y);

    b2Filter filter;
    filter.categoryBits = 0x0001;
    filter.maskBits = 0x0003;
    filter.groupIndex = 3;

    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.0f;
    fixtureDef.shape = &groundBox;
    fixtureDef.filter = filter;
    groundBody->CreateFixture(&fixtureDef);
    groundBody->SetUserData(&_bodyPair);
}

DemoLevel::DemoLevel()
{
}

void DemoLevel::Init(b2World* aWorld)
{
    _world = aWorld;
    //create background
    spSprite sky = new Sprite;
    sky->setResAnim(res::ui.getResAnim("sky"));
    sky->attachTo(this);

    setSize(getStage()->getSize().x*3, getStage()->getSize().y*2);

    spStatic ground = new Static(_world, RectF(getWidth() / 2, getHeight() - 10, getWidth() - 100, 30));
    addChild(ground);

    spSquare square = new Square(_world, Vector2(200, 300));
    square->attachTo(this);
    _squares.emplace_front(std::move(square));

    spSquare square2 = new Square(_world, Vector2(650, 300));
    square2->attachTo(this);
    _squares.emplace_front(std::move(square2));

    spSquare square3 = new Square(_world, Vector2(1100, 300));
    square3->attachTo(this);
    _squares.emplace_front(std::move(square3));

    addEventListener(TouchEvent::CLICK, CLOSURE(this, &DemoLevel::click));
}

void DemoLevel::doUpdate(const UpdateState& /*us*/)
{
    for(auto& circle : _circles)
    {
        assert(circle->IsAlive);

        circle->Update();

        if (circle->getY() > getHeight() + 50)
        {
            circle->_body->SetUserData(0);
            _world->DestroyBody(circle->_body);

            circle->detach();
            circle->IsAlive = false;
        }
    }

    for(auto& square : _squares)
    {
        square->Update();
    }

    _circles.remove_if([](spCircle circle) { return !circle->IsAlive; });
}

void DemoLevel::click(Event* event)
{
    std::cout << "demo level click" << std::endl;

    TouchEvent* te = safeCast<TouchEvent*>(event);

    // Background is target in this case, so removing if statement
    // for now. In future, need to read manuals for more inticate case.
//    if (event->target.get() == this)
//    {
        std::cout << "Creating circle!" << std::endl;
        spCircle circle = new Circle(_world, te->localPosition);
        circle->attachTo(this);
        _circles.push_front(circle);
//    }
}
