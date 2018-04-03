#include "Material.h"
#include "DemoLevel.hpp"
#include <iostream>

Square::Square(const Vector2& pos, float scale = 1)
{
    setResAnim(res::ui.getResAnim("square"));
    setAnchor(Vector2(0.5f, 0.5f));
    setTouchChildrenEnabled(false);

   setScale(scale);
}

void Square::Update()
{
}

void DemoLevel::Init(MapProperty&& a_MapProperty, const std::shared_ptr<ICollisionManager>& a_Manager)
{
    //create background
//    spSprite sky = new Sprite;
//    sky->setResAnim(res::ui.getResAnim("sky"));
//    sky->attachTo(this);

//    setSize(getStage()->getSize().x*3, getStage()->getSize().y*2);
    setSize(getStage()->getSize().x, getStage()->getSize().y);

//    spStatic ground = new Static(_world, RectF(getWidth() / 2, getHeight() - 10, getWidth() - 100, 30));
//    spStatic ground = new Static(RectF(0, getHeight()*0.5, getWidth() * 10, 30));
    spStatic ground = new Static(Rect(0, 60'000, 100'000, 3'000));
    addChild(ground);
    mStatic = ground.get();
    mObjects.push_back(ground);

    spStatic ground2 = new Static(Rect(90'000, 0, 5'000, 70'000));
    addChild(ground2);
    mStatic2 = ground2.get();
    mObjects.push_back(ground2);

    spStatic ground3 = new Static(Rect(0, 0, 5'000, 70'000));
    addChild(ground3);
    mStatic3 = ground3.get();
    mObjects.push_back(ground3);

    spStatic ground4 = new Static(Rect(30'000, 50'000, 40'000, 3'000));
    addChild(ground4);
    mStatic4 = ground4.get();
    mObjects.push_back(ground4);

    spStatic ground5 = new Static(Rect(20'000, 34'000, 15'000, 5'000));
    addChild(ground5);
    mStatic5 = ground5.get();
    mObjects.push_back(ground5);

    spPlatform platform1 = new Platform(Rect(60'000, 45'000, 25'000, 2'000), a_Manager);
    addChild(platform1);
    m_Platforms = std::make_unique<std::vector<spPlatform>>();
    m_Platforms->push_back(platform1);

//    spPlatform platform2 = new Platform(Rect(10'000, 45'000, 25'000, 2'000), a_Manager);
//    addChild(platform2);
//    m_Platforms->push_back(platform2);

//    m_DynamicObjects = std::make_unique<std::vector<spDynamicBox>>();
//    spDynamicBox box1 = new DynamicBox(Rect(72'000, 15'000, 10'000, 10'000), a_Manager);
//    addChild(box1);
//    m_DynamicObjects->push_back(box1);

//    std::cout << "After box1" << std::endl;
//    a_Manager->PrintCarrierId();

//    spDynamicBox box2 = new DynamicBox(Rect(72'000, -100'000, 10'000, 10'000), a_Manager);
//    addChild(box2);
//    m_DynamicObjects->push_back(box2);

    a_Manager->AddBody(mStatic);
    a_Manager->AddBody(mStatic2);
    a_Manager->AddBody(mStatic3);
    a_Manager->AddBody(mStatic4);
    a_Manager->AddBody(mStatic5);

    addEventListener(TouchEvent::CLICK, CLOSURE(this, &DemoLevel::click));

    // ------- TILED ---------
    mMapProperty = std::move(a_MapProperty);

    file::buffer fb;
    const std::string pathToTexture = "buch-outdoor.png";
    //load image from file
    Image src;
    file::read(pathToTexture, fb);
    src.init(fb, true);

    CreateTileSetTexture(src);

//    for (const MapObject& object : mMapProperty.mObjects)
//    {
////        mObjects.emplace_back(std::unique_ptr<Ground>(new Ground(mWorld, RectF(object.mX, object.mY, object.mWidth, object.mHeight))));
//        mObjects.emplace_back(new Static(mWorld, RectF(object.mX, object.mY, object.mWidth, object.mHeight)));
//    }
}

void DemoLevel::Update(const UpdateState& /*us*/)
{
    if (m_Platforms)
    {
        for (auto& platform : *m_Platforms)
        {
            platform->Update();
        }
    }

    if (m_DynamicObjects)
    {
        for (auto& boxes : *m_DynamicObjects)
        {
            boxes->Update();
        }
    }

    if (m_Platforms)
    {
        for (auto& platform : *m_Platforms)
        {
            platform->CheckCollisions();
        }
    }

    if (m_DynamicObjects)
    {
        for (auto& boxes : *m_DynamicObjects)
        {
            boxes->CheckCollisions();
        }
    }
}

void DemoLevel::SetPositions()
{
    if (m_Platforms)
    {
        for (auto& platform : *m_Platforms)
        {
            platform->SetPosition();
        }
    }

    if (m_DynamicObjects)
    {
        for (auto& boxes : *m_DynamicObjects)
        {
            boxes->SetPosition();
        }
    }
}

void DemoLevel::ToggleDebugDraw()
{
    if (m_Platforms)
    {
        for (auto& platform : *m_Platforms)
        {
            platform->ToggleDebugDraw();
        }
    }

    if (m_DynamicObjects)
    {
        for (auto& boxes : *m_DynamicObjects)
        {
            boxes->ToggleDebugDraw();
        }
    }
}

void DemoLevel::click(Event* /*event*/)
{
    std::cout << "demo level click" << std::endl;
}

void DemoLevel::CreateTileSetTexture(Image& src)
{
    // Size of batch file texture in tiles.
    mMapProperty.mTilesetCols = src.getWidth() / mMapProperty.mTileWidth;
    mMapProperty.mTilesetRows = src.getHeight() / mMapProperty.mTileHeight;

    Image dest;
    dest.init(mMapProperty.mTilesetCols * (mMapProperty.mTileWidth + 2), mMapProperty.mTilesetRows * (mMapProperty.mTileHeight * 2), TF_R8G8B8A8);

    //http://stackoverflow.com/questions/19611745/opengl-black-lines-in-between-tiles
    for (int y = 0; y < mMapProperty.mTilesetRows; ++y)
    {
        for (int x = 0; x < mMapProperty.mTilesetCols; ++x)
        {
            Rect srcRect(x * mMapProperty.mTileWidth, y * mMapProperty.mTileHeight, mMapProperty.mTileWidth, mMapProperty.mTileHeight);
            Rect destRect = srcRect;
            destRect.pos.x += 2 * x + 1;
            destRect.pos.y += 2 * y + 1;

            ImageData srcIm = src.lock(srcRect);
            ImageData destIm = dest.lock(destRect);
            operations::blit(srcIm, destIm);

            destRect.expand(Point(1, 1), Point(1, 1));

            operations::blit(
                dest.lock(destRect.pos.x + 1, destRect.pos.y + 1, mMapProperty.mTileWidth, 1),
                dest.lock(destRect.pos.x + 1, destRect.pos.y, mMapProperty.mTileWidth, 1));

            operations::blit(
                dest.lock(destRect.pos.x + 1, destRect.pos.y + mMapProperty.mTileHeight, mMapProperty.mTileWidth, 1),
                dest.lock(destRect.pos.x + 1, destRect.pos.y + mMapProperty.mTileHeight + 1, mMapProperty.mTileWidth, 1));

            operations::blit(
                dest.lock(destRect.pos.x + 1, destRect.pos.y, 1, mMapProperty.mTileHeight + 2),
                dest.lock(destRect.pos.x, destRect.pos.y, 1, mMapProperty.mTileHeight + 2));

            operations::blit(
                dest.lock(destRect.pos.x + mMapProperty.mTileWidth, destRect.pos.y, 1, mMapProperty.mTileHeight + 2),
                dest.lock(destRect.pos.x + mMapProperty.mTileWidth + 1, destRect.pos.y, 1, mMapProperty.mTileHeight + 2));
        }
    }

    mMapTexture = IVideoDriver::instance->createTexture();
    mMapTexture->init(dest.lock());
    mMapTexture->setClamp2Edge(true);
    mMapTexture->setLinearFilter(false);
}

void DemoLevel::drawLayer(int startX, int startY, int endX, int endY)
{
    Color color(Color::White);

    STDRenderer* renderer = STDRenderer::instance;

    float tw = 1.0f / mMapTexture->getWidth();
    float th = 1.0f / mMapTexture->getHeight();

    for (int y = startY; y < endY; ++y)
    {
        for (int x = startX; x < endX; ++x)
        {
            unsigned int tile = mMapProperty.mTilesPositions[y * mMapProperty.mMapWidth + x];
            if (!tile)
                continue;

            tile = tile - 1;

            int col = tile % mMapProperty.mTilesetCols;
            int row = tile / mMapProperty.mTilesetCols;

            Rect src(col * (mMapProperty.mTileWidth + 2) + 1, row * (mMapProperty.mTileHeight + 2) + 1, mMapProperty.mTileWidth, mMapProperty.mTileHeight);

            RectF srcUV = src.cast<RectF>();
            srcUV.pos.x *= tw;
            srcUV.pos.y *= th;
            srcUV.size.x *= tw;
            srcUV.size.y *= th;

            Rect dest(x * mMapProperty.mTileWidth, y * mMapProperty.mTileHeight, mMapProperty.mTileWidth, mMapProperty.mTileHeight);
            RectF destF = dest.cast<RectF>();
            renderer->draw(color, srcUV, destF);
        }
    }
}

void DemoLevel::doRender(const RenderState& rs)
{
    Material::setCurrent(rs.material);

    STDRenderer* renderer = STDRenderer::instance;
    renderer->setTexture(mMapTexture);
    renderer->setTransform(rs.transform);
    renderer->setBlendMode(blend_premultiplied_alpha);


    Transform world = rs.transform;
    world.invert();

    //find top left local position of TiledActor visible on display
    Vector2 topLeft = world.transform(Vector2(0, 0));

    //find bottom right local position of TiledActor visible on display
    Vector2 bottomRight = world.transform(getSize());

    //we don't want to draw tiles outside of visible area
    int startX = std::max(0,      int(topLeft.x / mMapProperty.mTileWidth));
    int startY = std::max(0,      int(topLeft.y / mMapProperty.mTileHeight));
    int endX   = std::min(mMapProperty.mMapWidth,  int(bottomRight.x / mMapProperty.mTileWidth) + 1);
    int endY   = std::min(mMapProperty.mMapHeight, int(bottomRight.y / mMapProperty.mTileHeight) + 1);

    drawLayer(startX, startY, endX, endY);
}


