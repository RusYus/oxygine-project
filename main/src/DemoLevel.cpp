#include "DemoLevel.hpp"
#include <iostream>

Circle::Circle(b2World* world, const Vector2& pos, float scale = 1)
    : _bodyPair(Service::ObjectType::DynamicBody, this)
{
    setResAnim(res::ui.getResAnim("circle"));
    setAnchor(Vector2(0.5f, 0.5f));
    setTouchChildrenEnabled(false);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = Service::Utils::Convert(pos);

    _body = world->CreateBody(&bodyDef);

    setUserData(_body);

    setScale(scale);

    b2CircleShape shape;
    shape.m_radius = getWidth() / Service::Constants::SCALE / 2 * scale;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 1.0f;
    fixtureDef.friction = 0.3f;

    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(&_bodyPair);
}

void Circle::Update()
{
    const b2Vec2& pos = _body->GetPosition();
    setPosition(Vector2(pos.x * 100, pos.y * 100));
    setRotation(_body->GetAngle());
}

Square::Square(b2World* world, const Vector2& pos, float scale = 1)
    : _bodyPair(Service::ObjectType::DynamicBody, this)
{
    setResAnim(res::ui.getResAnim("square"));
    setAnchor(Vector2(0.5f, 0.5f));
    setTouchChildrenEnabled(false);

    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position = Service::Utils::Convert(pos);
    bodyDef.fixedRotation = true;

    _body = world->CreateBody(&bodyDef);

    setUserData(_body);

    setScale(scale);

    b2PolygonShape shape;
    shape.SetAsBox(getWidth() / Service::Constants::SCALE / 2.0f, getHeight() / Service::Constants::SCALE / 2.0f);

    b2Filter filter;
    filter.categoryBits = 0x0003;
    filter.maskBits = 0x0001;
    filter.groupIndex = 2;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &shape;
    fixtureDef.density = 100.0f;
    fixtureDef.friction = 0.3f;
    fixtureDef.filter = filter;

    _body->CreateFixture(&fixtureDef);
    _body->SetUserData(&_bodyPair);
}

void Square::Update()
{
    const b2Vec2& pos = _body->GetPosition();
    setPosition(Vector2(pos.x * 100, pos.y * 100));
    setRotation(_body->GetAngle());
}

Static::Static(b2World* world, const RectF& rc)
    : _bodyPair(Service::ObjectType::Ground, this)
{
    setResAnim(res::ui.getResAnim("pen"));
    setSize(rc.getSize());
    setPosition(rc.getLeftTop());
    setAnchor(Vector2(0.5f, 0.5f));

    b2BodyDef groundBodyDef;
    groundBodyDef.position = Service::Utils::Convert(getPosition());

    b2Body* groundBody = world->CreateBody(&groundBodyDef);

    b2PolygonShape groundBox;
    b2Vec2 sz = Service::Utils::Convert(getSize() / 2);
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
    groundBody->SetUserData(&_bodyPair);
}

DemoLevel::DemoLevel()
{
}

void DemoLevel::Init(b2World* aWorld, Service::JsonImporter& aImporter)
{
    _world = aWorld;
    //create background
//    spSprite sky = new Sprite;
//    sky->setResAnim(res::ui.getResAnim("sky"));
//    sky->attachTo(this);

//    setSize(getStage()->getSize().x*3, getStage()->getSize().y*2);
    setSize(getStage()->getSize().x, getStage()->getSize().y);

    spStatic ground = new Static(_world, RectF(getWidth() / 2, getHeight() - 10, getWidth() - 100, 30));
    addChild(ground);

    spSquare square = new Square(_world, Vector2(200, 300));
    square->attachTo(this);
    _squares.emplace_front(std::move(square));

    spSquare square2 = new Square(_world, Vector2(650, 300));
    square2->attachTo(this);
    _squares.emplace_front(std::move(square2));

    spSquare square3 = new Square(_world, Vector2(1100, 300));
    square3->attachTo(this);
    _squares.emplace_front(std::move(square3));

    addEventListener(TouchEvent::CLICK, CLOSURE(this, &DemoLevel::click));

    // ------- TILED ---------
    _tileWidth = aImporter.GetTileWidth();
    _tileHeight = aImporter.GetTileHeight();
    _mapWidth = aImporter.GetMapWidth();
    _mapHeight = aImporter.GetMapHeight();
    _tilePositions = aImporter._tilePositions;

    file::buffer fb;
    const std::string pathToTexture = "buch-outdoor.png";
    //load image from file
    Image src;
    file::read(pathToTexture, fb);
    src.init(fb, true);

    CreateTileSetTexture(src);
}

void DemoLevel::doUpdate(const UpdateState& /*us*/)
{
    for(auto& circle : _circles)
    {
        assert(circle->IsAlive);

        circle->Update();

        if (circle->getY() > getHeight() + 50)
        {
            circle->_body->SetUserData(0);
            _world->DestroyBody(circle->_body);

            circle->detach();
            circle->IsAlive = false;
        }
    }

    for(auto& square : _squares)
    {
        square->Update();
    }

    _circles.remove_if([](spCircle circle) { return !circle->IsAlive; });
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
        spCircle circle = new Circle(_world, te->localPosition);
        circle->attachTo(this);
        _circles.push_front(circle);
//    }
}

void DemoLevel::CreateTileSetTexture(Image& src)
{
    // Size of batch file texture in tiles.
    cols = src.getWidth() / _tileWidth;
    rows = src.getHeight() / _tileHeight;

    Image dest;
    dest.init(cols * (_tileWidth + 2), rows * (_tileHeight * 2), TF_R8G8B8A8);

    //http://stackoverflow.com/questions/19611745/opengl-black-lines-in-between-tiles
    for (int y = 0; y < rows; ++y)
    {
        for (int x = 0; x < cols; ++x)
        {
            Rect srcRect(x * _tileWidth, y * _tileHeight, _tileWidth, _tileHeight);
            Rect destRect = srcRect;
            destRect.pos.x += 2 * x + 1;
            destRect.pos.y += 2 * y + 1;

            ImageData srcIm = src.lock(srcRect);
            ImageData destIm = dest.lock(destRect);
            operations::blit(srcIm, destIm);

            destRect.expand(Point(1, 1), Point(1, 1));

            operations::blit(
                dest.lock(destRect.pos.x + 1, destRect.pos.y + 1, _tileWidth, 1),
                dest.lock(destRect.pos.x + 1, destRect.pos.y, _tileWidth, 1));

            operations::blit(
                dest.lock(destRect.pos.x + 1, destRect.pos.y + _tileHeight, _tileWidth, 1),
                dest.lock(destRect.pos.x + 1, destRect.pos.y + _tileHeight + 1, _tileWidth, 1));

            operations::blit(
                dest.lock(destRect.pos.x + 1, destRect.pos.y, 1, _tileHeight + 2),
                dest.lock(destRect.pos.x, destRect.pos.y, 1, _tileHeight + 2));

            operations::blit(
                dest.lock(destRect.pos.x + _tileWidth, destRect.pos.y, 1, _tileHeight + 2),
                dest.lock(destRect.pos.x + _tileWidth + 1, destRect.pos.y, 1, _tileHeight + 2));
        }
    }

    _texture = IVideoDriver::instance->createTexture();
    _texture->init(dest.lock());
    _texture->setClamp2Edge(true);
    _texture->setLinearFilter(false);
}

void DemoLevel::drawLayer(int startX, int startY, int endX, int endY)
{
    Color color(Color::White);

    STDRenderer* renderer = STDRenderer::instance;

    float tw = 1.0f / _texture->getWidth();
    float th = 1.0f / _texture->getHeight();

    for (int y = startY; y < endY; ++y)
    {
        for (int x = startX; x < endX; ++x)
        {
            unsigned int tile = _tilePositions[y * _mapWidth + x];
            if (!tile)
                continue;

            tile = tile - 1;

            int col = tile % cols;
            int row = tile / cols;

            Rect src(col * (_tileWidth + 2) + 1, row * (_tileHeight + 2) + 1, _tileWidth, _tileHeight);

            RectF srcUV = src.cast<RectF>();
            srcUV.pos.x *= tw;
            srcUV.pos.y *= th;
            srcUV.size.x *= tw;
            srcUV.size.y *= th;

            Rect dest(x * _tileWidth, y * _tileHeight, _tileWidth, _tileHeight);
            RectF destF = dest.cast<RectF>();
            renderer->draw(color, srcUV, destF);
        }
    }
}

void DemoLevel::doRender(const RenderState& rs)
{
//        Material::setCurrent(rs.material);

    STDRenderer* renderer = STDRenderer::instance;
    renderer->setTexture(_texture);
    renderer->setTransform(rs.transform);
    renderer->setBlendMode(blend_premultiplied_alpha);


    Transform world = rs.transform;
    world.invert();

    //find top left local position of TiledActor visible on display
    Vector2 topLeft = world.transform(Vector2(0, 0));

    //find bottom right local position of TiledActor visible on display
    Vector2 bottomRight = world.transform(getSize());

    //we don't want to draw tiles outside of visible area
    int startX = std::max(0,      int(topLeft.x / _tileWidth));
    int startY = std::max(0,      int(topLeft.y / _tileHeight));
    int endX   = std::min(_mapWidth,  int(bottomRight.x / _tileWidth) + 1);
    int endY   = std::min(_mapHeight, int(bottomRight.y / _tileHeight) + 1);

    drawLayer(startX, startY, endX, endY);
}


