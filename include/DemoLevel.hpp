#pragma once

#include <forward_list>

#include "res.hpp"

#include "CollisionManager.hpp"
#include "DynamicBox.hpp"
#include "Ground.hpp"
#include "Platform.hpp"
#include "MapProperty.hpp"
#include "Utils.hpp"

DECLARE_SMART(Square, spSquare);
DECLARE_SMART(DemoLevel, spDemoLevel);

class Square : public Sprite, public Basis::BasisObject
{
public:
    Square(const Vector2&, float);
    void Update();
    bool IsAlive = true;
};

class DemoLevel: public Actor
{
public:
    void Init(MapProperty&&, const std::shared_ptr<ICollisionManager>&);
    void click(Event*);
    void Update(const UpdateState&);
    void SetPositions();
    void showHideDebug(Event*);
    Static* mStatic;
    Static* mStatic2;
    Static* mStatic3;
    Static* mStatic4;
    Static* mStatic5;
    spPlatform m_Platform;
    std::unique_ptr<std::vector<spDynamicBox>> m_DynamicObjects;

private:
    friend class Player;
    void doRender(const RenderState&);
    void drawLayer(int, int, int, int);
    void CreateTileSetTexture(Image&);

    spNativeTexture mMapTexture;
    MapProperty mMapProperty;
//    std::vector<std::unique_ptr<Ground>> mObjects;
    std::vector<spStatic> mObjects;
};
