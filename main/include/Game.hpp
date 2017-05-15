#pragma once

#include "Actor.h"
#include "Box2DDebugDraw.hpp"
#include "BasicCamera.hpp"
#include "Player.hpp"
#include "DemoLevel.hpp"
#include "Joystick.hpp"

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

        _levels.emplace_back(new DemoLevel);
        _levels.back()->Init(_world);
        addChild(_levels.back());

        _camera->setContent(_levels.back());

        //create virtual joystick
        _move = new Joystick;
        _move->attachTo(this);
        _move->setY(getStage()->getHeight() - _move->getHeight());

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
    }

    void doUpdate(const UpdateState& us)
    {
        //in real project you should make steps with fixed dt, check box2d documentation
        _world->Step(us.dt / 1000.0f, 6, 2);

        Vector2 dir;
        if (_move->getDirection(dir))
        {
            _player->Move(dir);
        }

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
    spJoystick _move;
    Content content;
    spBox2DDraw _debugDraw;
    std::vector<spDemoLevel> _levels;
};
