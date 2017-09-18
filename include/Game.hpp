#pragma once

#include "SDL_keyboard.h"

#include "Actor.h"
#include "key.h"
#include "CollisionManager.hpp"
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

// ----------- NEXT STEPS: ------------
// CollisionManager : calculates all collision (for each body using quad tree)
// Has references to all objects with body (taking from Game?)
// If something collides, report this to these bodies.
// Next, all bodies updates themselves, considering collisioninfo they've received.
// Should I use raycasting? E.g. CollisionManager says I collide with ground without details.
// Now I cast rays to ensure that and gather other info (which side, etc.)
// CollisionMask? For the future, not now
// Number of rays, their spacing, depends on width and height of the body, length depends on velocity in that direction
// Rays are casted towards moving direction, except directly down (ground), which casted everytime.


// TODO [MOV2-47]
// Add coordinates as arguments to ctors for all objects (player, platform, etc.)
// In ctor set position (to set position m_View)
// Using everywhere Vector2L with cast to Vector2 if neccesary (length() and normalize())
// Set position for m_View only for graphics
// Use scale to convert between Vector2L and Vector2 (in setting m_View)

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
        mLevels.back()->Init(std::move(mapProperty));
        addChild(mLevels.back());

        mCamera->setContent(mLevels.back());

        //create player ship
        m_Player = new Player;
        m_Player->Init(mEventProxy);
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

        m_CollisionManager->AddBody(m_Player.get());
        m_CollisionManager->AddBody(mLevels.back()->mStatic);
        m_CollisionManager->AddBody(mLevels.back()->mStatic2);
        m_CollisionManager->AddBody(mLevels.back()->mStatic3);
        m_CollisionManager->AddBody(mLevels.back()->mStatic4);
        m_CollisionManager->AddBody(mLevels.back()->m_Platform.get());

        m_Player->BindCollisionManager(m_CollisionManager);
        mLevels.back()->m_Platform->BindCollisionManager(m_CollisionManager);
    }

    void doUpdate(const UpdateState& us)
    {
//        std::cout << "------Starting Step:"<< std::endl;
        m_Player->Update(us);
        mLevels.back()->Update(us);
        mLevels.back()->m_Platform->CheckCollisions();
        m_Player->CheckCollisions();
        mLevels.back()->m_Platform->SetPosition();
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
