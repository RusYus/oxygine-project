#include "Material.h"
#include "DemoLevel.hpp"
#include <iostream>

Circle::Circle(const Vector2& pos, float scale = 1)
    : mBodyPair(Service::ObjectType::DynamicBody, this)
{
    setResAnim(res::ui.getResAnim("circle"));
    setAnchor(Vector2(0.5f, 0.5f));
    setTouchChildrenEnabled(false);

    setScale(scale);
}

void Circle::Update()
{
}

Square::Square(const Vector2& pos, float scale = 1)
    : mBodyPair(Service::ObjectType::DynamicBody, this)
{
    setResAnim(res::ui.getResAnim("square"));
    setAnchor(Vector2(0.5f, 0.5f));
    setTouchChildrenEnabled(false);

   setScale(scale);
}

void Square::Update()
{
}

SquareMovable::SquareMovable(const Vector2& pos, float scale = 1)
    : mBodyPair(Service::ObjectType::DynamicBody, this)
{
    setResAnim(res::ui.getResAnim("square"));
    setAnchor(Vector2(0.5f, 0.5f));
    setTouchChildrenEnabled(false);

    setScale(scale);
}

void SquareMovable::Update()
{
//    mBody->SetLinearVelocity(b2Vec2{-2, 0});
//    const b2Vec2& pos = mBody->GetPosition();
////    std::cout << "Square:" << pos.x*100 << "    Vel:" << mBody->GetLinearVelocity().x << std::endl;
//    setPosition(Vector2(pos.x * 100, pos.y * 100));
//    setRotation(mBody->GetAngle());

//    b2MassData m;
//    mBody->GetMassData(&m);
//    std::cout << "Mass:" << m.center.x << ":" << m.center.y << ";" << m.mass << std::endl;
}

//Static::Static(const RectF& rc)
//    : mBodyPair(Service::ObjectType::Ground, this)
//{
//    setResAnim(res::ui.getResAnim("pen"));
//    setSize(rc.getSize());
////    setPosition(rc.getLeftTop());
//    setPosition(rc.getLeftTop());
////    setAnchor(Vector2(0.5f, 0.5f));

////    b2BodyDef groundBodyDef;
////    groundBodyDef.position = Service::Utils::Convert(getPosition());

////    b2Body* groundBody = world->CreateBody(&groundBodyDef);

////    b2PolygonShape groundBox;
////    b2Vec2 sz = Service::Utils::Convert(getSize() / 2);
////    groundBox.SetAsBox(sz.x, sz.y);

////    b2Filter filter;
////    filter.categoryBits = 0x0001;
////    filter.maskBits = 0x0003;
////    filter.groupIndex = 3;

////    b2FixtureDef fixtureDef;
////    fixtureDef.density = 0.0f;
////    fixtureDef.shape = &groundBox;
////    fixtureDef.filter = filter;
////    groundBody->CreateFixture(&fixtureDef);
////    groundBody->SetUserData(&mBodyPair);
//}

Ground::Ground(b2World* world, const RectF& rc)
    : mBodyPair(Service::ObjectType::Ground, this)
{
    b2BodyDef groundBodyDef;
    groundBodyDef.type = b2_staticBody;
    groundBodyDef.position = Service::Utils::Convert(rc.getLeftTop());

    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    b2Vec2 sz = Service::Utils::Convert(rc.getSize() / 2);
    groundBox.SetAsBox(sz.x, sz.y);

    b2Filter filter;
    filter.categoryBits = 0x0001;
    filter.maskBits = 0x0003;
    filter.groupIndex = 3;

    b2FixtureDef fixtureDef;
    fixtureDef.density = 0.0f;
    fixtureDef.shape = &groundBox;
    fixtureDef.filter = filter;
    groundBody->CreateFixture(&fixtureDef);
    groundBody->SetUserData(&mBodyPair);
}

void DemoLevel::Init(b2World* aWorld, MapProperty&& aMapProperty)
{
    mWorld = aWorld;
    //create background
//    spSprite sky = new Sprite;
//    sky->setResAnim(res::ui.getResAnim("sky"));
//    sky->attachTo(this);

//    setSize(getStage()->getSize().x*3, getStage()->getSize().y*2);
    setSize(getStage()->getSize().x, getStage()->getSize().y);

//    spStatic ground = new Static(_world, RectF(getWidth() / 2, getHeight() - 10, getWidth() - 100, 30));
//    spStatic ground = new Static(RectF(0, getHeight()*0.5, getWidth() * 10, 30));
    spStatic ground = new Static(RectF(0, 600, 1000, 30));
    addChild(ground);
    mStatic = ground.get();
    mObjects.push_back(ground);

//    spStatic ground2 = new Static(RectF(900, 0, 50, 700));
//    addChild(ground2);
//    mStatic2 = ground2.get();
//    mObjects.push_back(ground2);

//    spStatic ground3 = new Static(RectF(0, 0, 50, 700));
//    addChild(ground3);
//    mStatic3 = ground3.get();
//    mObjects.push_back(ground3);

//    spStatic ground4 = new Static(RectF(300, 500, 400, 30));
//    addChild(ground4);
//    mStatic4 = ground4.get();
//    mObjects.push_back(ground4);

//    spGround groundW = new Ground(mWorld, RectF(800, 200, 400, 50));
//    addChild(groundW);

//    spSquare square = new Square(mWorld, Vector2(200, 300));
//    square->attachTo(this);
//    mSquares.emplace_front(std::move(square));

//    spSquare square2 = new Square(mWorld, Vector2(650, 300));
//    square2->attachTo(this);
//    mSquares.emplace_front(std::move(square2));

//    spSquare square3 = new Square(mWorld, Vector2(1100, 300));
//    square3->attachTo(this);
//    mSquares.emplace_front(std::move(square3));

    addEventListener(TouchEvent::CLICK, CLOSURE(this, &DemoLevel::click));

    // ------- TILED ---------
    mMapProperty = std::move(aMapProperty);

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

//    spPlatform platform = new Platform(RectF(400, 200, 300, 40));
//    addChild(platform);
//    m_Platform = platform;
}

void DemoLevel::doUpdate(const UpdateState& /*us*/)
{
    for(auto& circle : mCircles)
    {
        assert(circle->IsAlive);

        circle->Update();

        if (circle->getY() > getHeight() + 50)
        {
            circle->detach();
            circle->IsAlive = false;
        }
    }

    for(auto& square : mSquares)
    {
        square->Update();
    }

    mCircles.remove_if([](spCircle circle) { return !circle->IsAlive; });

    if (m_Platform)
    {
        m_Platform->Move();
    }
}

void DemoLevel::click(Event* event)
{
    std::cout << "demo level click" << std::endl;

    TouchEvent* te = safeCast<TouchEvent*>(event);

    // Background is target in this case, so removing if statement
    // for now. In future, need to read manuals for more intricate case.
//    if (event->target.get() == this)
//    {
        std::cout << "Creating circle!" << std::endl;
//        spCircle circle = new Circle(mWorld, te->localPosition);
//        circle->attachTo(this);
//        mCircles.push_front(circle);
        spSquareMovable s = new SquareMovable(te->localPosition);
        s->attachTo(this);
        mSquares.push_front(s);
//    }
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


