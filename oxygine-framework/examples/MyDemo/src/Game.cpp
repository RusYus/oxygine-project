#include "Game.h"
#include "Player.h"
#include "Joystick.h"
#include "res.h"
#include <iostream>
#include <typeinfo>

const float SCALE = 100.0f;
b2Vec2 convert(const Vector2& pos)
{
    return b2Vec2(pos.x / SCALE, pos.y / SCALE);
}

Vector2 convert(const b2Vec2& pos)
{
    return Vector2(pos.x * SCALE, pos.y * SCALE);
}

void PlayerListener::BeginContact(b2Contact* contact)
{
    std::cout << "BeginContact" << std::endl;
    auto* fixtureAData = contact->GetFixtureA()->GetBody()->GetUserData();
    if(fixtureAData)
    {
        static_cast<Player*>(fixtureAData)->SetMove(false);
        std::cout << "STOP" << std::endl;
    }

    auto* fixtureBData = contact->GetFixtureB()->GetBody()->GetUserData();
    if(fixtureBData)
    {
        static_cast<Player*>(fixtureBData)->SetMove(false);
        std::cout << "STOP" << std::endl;
    }
}

void PlayerListener::EndContact(b2Contact* contact)
{
    std::cout << "EndContact" << std::endl;
    auto* fixtureAData = contact->GetFixtureA()->GetBody()->GetUserData();
    if(fixtureAData)
    {
        static_cast<Player*>(fixtureAData)->SetMove(true);
        std::cout << "GO" << std::endl;
    }

    auto* fixtureBData = contact->GetFixtureB()->GetBody()->GetUserData();
    if(fixtureBData)
    {
        static_cast<Player*>(fixtureBData)->SetMove(true);
        std::cout << "GO" << std::endl;
    }
}

Circle::Circle(b2World* world, const Vector2& pos, float scale = 1)
{
    setResAnim(res::ui.getResAnim("circle"));
    setAnchor(Vector2(0.5f, 0.5f));
    setTouchChildrenEnabled(false);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = convert(pos);

    _body = world->CreateBody(&bodyDef);

    setUserData(_body);

    setScale(scale);

    b2CircleShape shape;
    shape.m_radius = getWidth() / SCALE / 2 * scale;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(this);
}

Static::Static(b2World* world, const RectF& rc)
{
    //setHorizontalMode(Box9Sprite::TILING_FULL);
    //setVerticalMode(Box9Sprite::TILING_FULL);
    setResAnim(res::ui.getResAnim("pen"));
    setSize(rc.getSize());
    setPosition(rc.getLeftTop());
    setAnchor(Vector2(0.5f, 0.5f));

    b2BodyDef groundBodyDef;
    groundBodyDef.position = convert(getPosition());

    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    b2Vec2 sz = convert(getSize() / 2);
    groundBox.SetAsBox(sz.x, sz.y);
    groundBody->CreateFixture(&groundBox, 0.0f);
}

Game::Game()
//    : _world(0)
//    , _pl()
{
}

void Game::init()
{
//    //scene layer would have size of display
//    setSize(getStage()->getSize());

//    //create background
////    spSprite sky = new Sprite;
////    sky->setResAnim(res::ui.getResAnim("sky"));
////    sky->attachTo(this);

//    // From Box2D.

//    spButton btn = new Button;
//    btn->setX(getWidth() - btn->getWidth() - 3);
//    btn->setY(3);
//    btn->attachTo(this);
//    btn->addEventListener(TouchEvent::CLICK, CLOSURE(this, &Game::showHideDebug));

//    addEventListener(TouchEvent::CLICK, CLOSURE(this, &Game::click));

//    _world = new b2World(b2Vec2(0, 10));

//    spStatic ground = new Static(_world, RectF(getWidth() / 2, getHeight() - 10, getWidth() - 100, 30));
//    addChild(ground);

//    spCircle circle = new Circle(_world, getSize() / 2, 1);
//    addChild(circle);



//    _world->SetContactListener(&_pl);
//    _world->SetAllowSleeping(false);


    setSize(getStage()->getSize());

    _world = new b2World(b2Vec2(0, 10));

    //create player ship
    _player = new Player;
    _player->init(this);
    _player->_init(_world);

    //create virtual joystick
    _move = new Joystick;
    _move->attachTo(this);
    _move->setY(getHeight() - _move->getHeight());

    spButton btn = new Button;
    btn->setX(getWidth() - btn->getWidth() - 3);
    btn->setY(3);
    btn->attachTo(this);
    btn->addEventListener(TouchEvent::CLICK, CLOSURE(this, &Game::showHideDebug));

    addEventListener(TouchEvent::CLICK, CLOSURE(this, &Game::click));




    spStatic ground = new Static(_world, RectF(getWidth() / 2, getHeight() - 10, getWidth() - 100, 30));
    addChild(ground);

//    spCircle circle = new Circle(_world, getSize() / 2, 1);
//    addChild(circle);
}

void Game::doUpdate(const UpdateState& us)
{
    //update player each frame
    //in real project you should make steps with fixed dt, check box2d documentation
     _world->Step(us.dt / 1000.0f, 6, 2);

     _player->update(us);

     for(auto& circle : _circles)
     {
         if(!circle->IsAlive)
         {
             continue;
         }
         circle->Update();
         if (circle->getY() > getHeight() + 50)
        {
          circle->_body->SetUserData(0);
          _world->DestroyBody(circle->_body);

          circle->detach();
          circle->IsAlive = false;
        }
     }
     //update each body position on display
//     b2Body* body = _world->GetBodyList();
//     while (body)
//     {
//         std::cout << "start step" << std::endl;
//         b2Body* next;
//         std::cout << "N" << std::endl;
//         Actor* actor = (Actor*)body->GetUserData();
//         next = body->GetNext();
//         if (actor)
//         {
//             const b2Vec2& pos = body->GetPosition();
//             actor->setPosition(convert(pos));
//             actor->setRotation(body->GetAngle());

//             std::cout << "after" << std::endl;

//             //remove fallen bodies
//        //                 if (actor->getY() > getHeight() + 50)
//        //                 {
//        //                     body->SetUserData(0);
//        //                     _world->DestroyBody(body);

//        //                     actor->detach();
//        //                 }
//         }


//         body = next;
//        std::cout << "end step" << std::endl;
//     }
}

void Game::showHideDebug(Event* event)
{
    TouchEvent* te = safeCast<TouchEvent*>(event);
    te->stopsImmediatePropagation = true;
    if (_debugDraw)
    {
        _debugDraw->detach();
        _debugDraw = 0;
        return;
    }

    _debugDraw = new Box2DDraw;
    _debugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
    _debugDraw->attachTo(this);
    _debugDraw->setWorld(SCALE, _world);
    _debugDraw->setPriority(1);
}

void Game::click(Event* event)
{
    TouchEvent* te = safeCast<TouchEvent*>(event);

    if (event->target.get() == this)
    {
        spCircle circle = new Circle(_world, te->localPosition);
//        spCircle circle = new Circle(_world, Vector2(775, 430));
//        std::cout << "click " << te->localPosition.x << ":" << te->localPosition.y << std::endl;
        circle->attachTo(this);
        _circles.push_back(circle);
    }

//    if (event->target->getUserData())
//    {
//        //shot to circle
//        spActor actor = safeSpCast<Actor>(event->target);
//        b2Body* body = (b2Body*)actor->getUserData();

//        Vector2 dir = actor->getPosition() - te->localPosition;
//        dir = dir / dir.length() * body->GetMass() * 200;

//        body->ApplyForceToCenter(b2Vec2(dir.x, dir.y), true);

//        spSprite sprite = new Sprite();
//        sprite->setResAnim(res::ui.getResAnim("shot"));
//        Vector2 local = actor->parent2local(te->localPosition);
//        sprite->setPosition(local);
//        sprite->setAnchor(Vector2(0.5f, 0.5f));
//        sprite->attachTo(actor);
//    }
}
