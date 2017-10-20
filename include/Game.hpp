#pragma once

#include "SDL_keyboard.h"

#include "Actor.h"
#include "key.h"
#include "BasisObject.hpp"
#include "BasisCamera.hpp"
#include "Player.hpp"
#include "DemoLevel.hpp"
#include "MovementButton.hpp"
#include "JsonImporter.hpp"

#include <iostream>
#include <vector>

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
        : mContent()
        , mEventProxy(new EventProxy)
        , m_CollisionManager(std::make_shared<CollisionManager>())
    {
        oxygine::key::init();

        MapProperty mapProperty;
        mImporter = std::unique_ptr<Service::JsonImporter>(new Service::JsonImporter());
        if (!mImporter->LoadMap("1.json", mapProperty))
        {
            std::cout << "Couldn't load map!" << std::endl;
            return;
        }

        mCamera = new Camera(mEventProxy);
        mCamera->attachTo(&mContent);
        mCamera->setSize(getStage()->getSize());
        addChild(mCamera);

        mLevels.emplace_back(new DemoLevel);
        mLevels.back()->Init(std::move(mapProperty), m_CollisionManager);
        addChild(mLevels.back());

        mCamera->setContent(mLevels.back());

        //create player ship
        m_Player = new Player(mEventProxy, m_CollisionManager);
//        mLevels.back()->addChild(mPlayer->GetView());
        mLevels.back()->addChild(m_Player);

        // TODO : camera not changing coordinates.
//        _camera->setX(_player->GetX() - _camera->getWidth() / 2.0);
//        _camera->setY(_player->GetY() - _camera->getHeight() * 0.01);


        spButton btn = new Button;
        btn->setX(getStage()->getWidth() - btn->getWidth() - 3);
        btn->setY(3);
        btn->attachTo(this);
        btn->addEventListener(TouchEvent::CLICK, CLOSURE(this, &Game::ShowHideDebug));

        mMoveLeft = new MovementButton(false, mEventProxy);
        mMoveLeft->setWidth(100);
        mMoveLeft->setHeight(100);
        mMoveLeft->setX(10);
        mMoveLeft->setY(getStage()->getHeight() - mMoveLeft->getHeight() - 10);
        mMoveLeft->attachTo(this);

        mMoveRight = new MovementButton(true, mEventProxy);
        mMoveRight->setWidth(100);
        mMoveRight->setHeight(100);
        mMoveRight->setX(10 + mMoveLeft->getX() + mMoveLeft->getWidth());
        mMoveRight->setY(mMoveLeft->getY());
        mMoveRight->attachTo(this);

        mJump = new JumpButton(mEventProxy);
        mJump->setWidth(100);
        mJump->setHeight(100);
        mJump->setX(getStage()->getWidth() - mJump->getWidth() - 10);
        mJump->setY(getStage()->getHeight() - mJump->getHeight() - 10);
        mJump->attachTo(this);
    }

    void doUpdate(const UpdateState& us)
    {
        std::cout << "------Starting Step:"<< std::endl;
        m_Player->Update(us);
        mLevels.back()->Update(us);
        m_Player->CheckCollisions();
        mLevels.back()->SetPositions();
        m_Player->SetPosition();
    }

    void ShowHideDebug(Event* /*event*/)
    {
        m_Player->SetDebugDraw(!m_Player->GetDebugDraw());
        mLevels.back()->m_Platform->SetDebugDraw(!mLevels.back()->m_Platform->GetDebugDraw());
    }

    spEventProxy mEventProxy;
    std::shared_ptr<CollisionManager> m_CollisionManager;
    spPlayer m_Player;
    spCamera mCamera;
    spMoveButton mMoveLeft;
    spMoveButton mMoveRight;
    spJumpButton mJump;
    Content mContent;
    std::unique_ptr<Service::JsonImporter> mImporter;
    std::vector<spDemoLevel> mLevels;
};
