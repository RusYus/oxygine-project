#pragma once

#include "Actor.h"
#include "BasisObject.hpp"
#include "Box2DDebugDraw.hpp"
#include "BasisCamera.hpp"
#include "Player.hpp"
#include "DemoLevel.hpp"
#include "MovementButton.hpp"
#include "JsonImporter.hpp"

#include <iostream>
#include <vector>

using namespace oxygine;

class Content: public Actor
{
public:
    Content() : driver(0) { setName("content"); }
    IVideoDriver* driver;

    /*
    void render(const RenderState& parentRS)
    {

        parentRS.renderer->drawBatch();

        RenderState rs = parentRS;
        STDRenderer renderer(driver ? driver : IVideoDriver::instance);
        renderer.Renderer::begin(parentRS.renderer);
        rs.renderer = &renderer;
        Actor::render(rs);
        renderer.end();
    }
    */
};

class ContactFilterWrapper : public b2ContactFilter
{
    bool ShouldCollide(b2Fixture* fixtureA, b2Fixture* fixtureB)
    {
        const b2Filter& filterA = fixtureA->GetFilterData();
        const b2Filter& filterB = fixtureB->GetFilterData();

        std::cout << "FilterA:" << filterA.categoryBits
                  << ";" << filterA.maskBits << ";" << filterA.groupIndex << std::endl;
        std::cout << "FilterB:" << filterB.categoryBits
                  << ";" << filterB.maskBits << ";" << filterB.groupIndex << std::endl;
        if (filterA.groupIndex == filterB.groupIndex && filterA.groupIndex != 0)
        {
            return filterA.groupIndex > 0;
        }

        bool collide = (filterA.maskBits & filterB.categoryBits) != 0 &&
                        (filterA.categoryBits & filterB.maskBits) != 0;
        return collide;
//        return true;
    }
};

class ContactListenerWrapper : public b2ContactListener
{
    void BeginContact(b2Contact* contact)
    {
        auto userDataA = static_cast<std::pair<Service::ObjectType, BasisObject*>*>(contact->GetFixtureA()->GetBody()->GetUserData());
        auto userDataB = static_cast<std::pair<Service::ObjectType, void*>*>(contact->GetFixtureB()->GetBody()->GetUserData());
        if ((userDataA->first == Service::ObjectType::DynamicBody && userDataB->first == Service::ObjectType::Player)
            || (userDataA->first == Service::ObjectType::Player && userDataB->first == Service::ObjectType::DynamicBody))
        {
            if (userDataA->first == Service::ObjectType::Player)
            {
                static_cast<Player*>(userDataA->second)->SetNormal(contact->GetManifold()->localNormal);
            }

            if (userDataB->first == Service::ObjectType::Player)
            {
                static_cast<Player*>(userDataB->second)->SetNormal(contact->GetManifold()->localNormal);
            }
        }
    }

    void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse)
    {
        auto userDataA = static_cast<std::pair<Service::ObjectType, BasisObject*>*>(contact->GetFixtureA()->GetBody()->GetUserData());
        auto userDataB = static_cast<std::pair<Service::ObjectType, void*>*>(contact->GetFixtureB()->GetBody()->GetUserData());
        if ((userDataA->first == Service::ObjectType::DynamicBody && userDataB->first == Service::ObjectType::Player)
            || (userDataA->first == Service::ObjectType::Player && userDataB->first == Service::ObjectType::DynamicBody))
        {
            contact->SetEnabled(false);

            if (userDataA->first == Service::ObjectType::DynamicBody)
            {
                // For future use, need to add every Service::ObjectType for each movable object
                // And cast it accordingly (move to function I think with a lot of switches).
                static_cast<Circle*>(userDataA->second)->_body->SetLinearVelocity(b2Vec2(0, 0));
            }

            if (userDataB->first == Service::ObjectType::DynamicBody)
            {
                static_cast<Circle*>(userDataB->second)->_body->SetLinearVelocity(b2Vec2(0, 0));
            }
        }
    }
};

DECLARE_SMART(Game, spGame);
class Game: public Actor
{
public:
    Game()
        : content()
        , _eventProxy(new EventProxy)
    {
        _world = new b2World(b2Vec2(0, 10));

        _importer = std::unique_ptr<Service::JsonImporter>(new Service::JsonImporter());
        if (!_importer->LoadMap("/home/ruslan/Work/Project/oxygine-project/build-proj.cmake-Desktop-Debug/1.json"))
        {
            std::cout << "Couldn't load map!" << std::endl;
            return;
        }

        _camera = new Camera(_eventProxy);
        _camera->attachTo(&content);
        _camera->setSize(getStage()->getSize());
        addChild(_camera);

        _levels.emplace_back(new DemoLevel);
        _levels.back()->Init(_world, *_importer);
        addChild(_levels.back());

        _camera->setContent(_levels.back());

        //create player ship
        _player = new Player;
        _player->Init(_world, _eventProxy);
        _levels.back()->addChild(_player->GetView());

        // TODO : camera not changing coordinates.
//        _camera->setX(_player->GetX() - _camera->getWidth() / 2.0);
//        _camera->setY(_player->GetY() - _camera->getHeight() * 0.01);


        spButton btn = new Button;
        btn->setX(getStage()->getWidth() - btn->getWidth() - 3);
        btn->setY(3);
        btn->attachTo(this);
        btn->addEventListener(TouchEvent::CLICK, CLOSURE(this, &Game::ShowHideDebug));

        _moveLeft = new MovementButton(false, _eventProxy);
        _moveLeft->setWidth(100);
        _moveLeft->setHeight(100);
        _moveLeft->setX(10);
        _moveLeft->setY(getStage()->getHeight() - _moveLeft->getHeight() - 10);
        _moveLeft->attachTo(this);

        _moveRight = new MovementButton(true, _eventProxy);
        _moveRight->setWidth(100);
        _moveRight->setHeight(100);
        _moveRight->setX(10 + _moveLeft->getX() + _moveLeft->getWidth());
        _moveRight->setY(_moveLeft->getY());
        _moveRight->attachTo(this);

        _jump = new JumpButton(_eventProxy);
        _jump->setWidth(100);
        _jump->setHeight(100);
        _jump->setX(getStage()->getWidth() - _jump->getWidth() - 10);
        _jump->setY(getStage()->getHeight() - _jump->getHeight() - 10);
        _jump->attachTo(this);

        _world->SetContactFilter(&_cf);
        _world->SetContactListener(&_cl);
    }

    void doUpdate(const UpdateState& us)
    {
        //in real project you should make steps with fixed dt, check box2d documentation
        _world->Step(us.dt / 1000.0f, 6, 2);
        _player->Update(us);
    }

    // TODO : Not working right now.
    void ShowHideDebug(Event* event)
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
        _debugDraw->attachTo(_levels.back());
        _debugDraw->setWorld(100, _world);
        _debugDraw->setPriority(1);
    }

    spEventProxy _eventProxy;
    b2World* _world;
    spPlayer _player;
    spCamera _camera;
    spMoveButton _moveLeft;
    spMoveButton _moveRight;
    spJumpButton _jump;
    Content content;
    ContactFilterWrapper _cf;
    ContactListenerWrapper _cl;
    spBox2DDraw _debugDraw;
    std::unique_ptr<Service::JsonImporter> _importer;
    std::vector<spDemoLevel> _levels;
};
