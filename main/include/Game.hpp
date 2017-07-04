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
                static_cast<Circle*>(userDataA->second)->mBody->SetLinearVelocity(b2Vec2(0, 0));
            }

            if (userDataB->first == Service::ObjectType::DynamicBody)
            {
                static_cast<Circle*>(userDataB->second)->mBody->SetLinearVelocity(b2Vec2(0, 0));
            }
        }
    }
};

DECLARE_SMART(Game, spGame);
class Game: public Actor
{
public:
    Game()
        : mContent()
        , mEventProxy(new EventProxy)
    {
        mWorld = new b2World(b2Vec2(0, 10));

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
        mLevels.back()->Init(mWorld, std::move(mapProperty));
        addChild(mLevels.back());

        mCamera->setContent(mLevels.back());

        //create player ship
        mPlayer = new Player;
        mPlayer->Init(mWorld, mEventProxy);
        mLevels.back()->addChild(mPlayer->GetView());

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

        mWorld->SetContactFilter(&mCf);
        mWorld->SetContactListener(&mCl);
    }

    void doUpdate(const UpdateState& us)
    {
        //in real project you should make steps with fixed dt, check box2d documentation
        mWorld->Step(us.dt / 1000.0f, 6, 2);
        mPlayer->Update(us);
    }

    // TODO : Not working right now.
    void ShowHideDebug(Event* event)
    {
        TouchEvent* te = safeCast<TouchEvent*>(event);
        te->stopsImmediatePropagation = true;
        if (mDebugDraw)
        {
            mDebugDraw->detach();
            mDebugDraw = 0;
            return;
        }

        mDebugDraw = new Box2DDraw;
        mDebugDraw->SetFlags(b2Draw::e_shapeBit | b2Draw::e_jointBit | b2Draw::e_pairBit | b2Draw::e_centerOfMassBit);
        mDebugDraw->attachTo(mLevels.back());
        mDebugDraw->setWorld(100, mWorld);
        mDebugDraw->setPriority(1);
    }

    spEventProxy mEventProxy;
    b2World* mWorld;
    spPlayer mPlayer;
    spCamera mCamera;
    spMoveButton mMoveLeft;
    spMoveButton mMoveRight;
    spJumpButton mJump;
    Content mContent;
    ContactFilterWrapper mCf;
    ContactListenerWrapper mCl;
    spBox2DDraw mDebugDraw;
    std::unique_ptr<Service::JsonImporter> mImporter;
    std::vector<spDemoLevel> mLevels;
};
