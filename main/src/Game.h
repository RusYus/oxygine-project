#pragma once
#include "Actor.h"
#include "BasicCamera.h"
#include "DemoLevel.h"

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

        demoLevel->init(_eventProxy);
        addChild(demoLevel);
    }

    spEventProxy _eventProxy;
    Content content;
};
