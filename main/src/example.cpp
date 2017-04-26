#include "oxygine-framework.h"
#include "res.h"
#include "DemoLevel.h"
using namespace oxygine;

void example_preinit()
{
}

void example_init()
{
    //load resources
    res::load();

    //create scene with game
    spDemoLevel demoLevel = new DemoLevel;
    demoLevel->init();
    demoLevel->attachTo(getStage());
}

void example_update()
{

}

void example_destroy()
{
    res::free();
}
