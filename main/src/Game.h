#pragma once
#include "Actor.h"
#include "BasicCamera.h"
#include "Player.h"
#include "DemoLevel.h"
#include "Joystick.h"

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

DECLARE_SMART(Game, spGame);
class Game: public Actor
{
public:
    Game()
        : content()
        , _eventProxy(new EventProxy)
    {
        spCamera cam = new Camera(_eventProxy);
        cam->attachTo(&content);
        // TODO : figure out what's wrong with 0 size
//        cam->setSize(content.getSize());
        // TODO : level's touch event overrides camera's
        cam->setSize(getStage()->getSize());
        addChild(cam);

        spDemoLevel demoLevel = new DemoLevel;

        cam->setContent(demoLevel);

        demoLevel->init();
        addChild(demoLevel);

        //create virtual joystick
        _move = new Joystick;
        _move->attachTo(this);
        _move->setY(demoLevel->getHeight() - _move->getHeight());

        std::cout << "MOVE:" << _move->getX() << " : " << _move->getY() << std::endl;

        //create player ship
        _player = new Player;
        _player->Init(demoLevel, _eventProxy);
    }

    void doUpdate(const UpdateState& us)
    {
        Vector2 dir;
        if (_move->getDirection(dir))
        {
            _player->Move(dir);
        }

        _player->Update(us);
    }

    spEventProxy _eventProxy;
    spPlayer _player;
    spJoystick _move;
    Content content;
};
