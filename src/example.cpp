#include "oxygine-framework.h"
#include "res.hpp"
#include "Game.hpp"

using namespace oxygine;

void example_preinit()
{
}

void example_init()
{
    //load resources
    res::load();

    //create scene with game
    spGame game = new Game;
    game->attachTo(getStage());
}

void example_update()
{

}

void example_destroy()
{
    res::free();
}
