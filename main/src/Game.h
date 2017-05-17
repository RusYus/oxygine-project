#pragma once

#include "Actor.h"
#include "Box2DDebugDraw.h"
#include "BasicCamera.h"
#include "Player.h"
#include "DemoLevel.h"
#include "MovementButton.h"

#include <iostream>

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

class ContactFilter : public b2ContactFilter
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

DECLARE_SMART(Game, spGame);
class Game: public Actor
{
public:
    Game()
        : content()
        , _eventProxy(new EventProxy)
    {
        _world = new b2World(b2Vec2(0, 10));

        _camera = new Camera(_eventProxy);
        _camera->attachTo(&content);
        _camera->setSize(getStage()->getSize());
        addChild(_camera);

        spDemoLevel demoLevel = new DemoLevel;

        _camera->setContent(demoLevel);

        demoLevel->Init(_world);
        addChild(demoLevel);

        //create player ship
        _player = new Player;
        _player->Init(demoLevel, _eventProxy);

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
        _debugDraw->attachTo(this);
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
    ContactFilter _cf;
    spBox2DDraw _debugDraw;
};
