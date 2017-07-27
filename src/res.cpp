#include "res.hpp"

namespace res
{
    Resources ui;

    void load()
    {
        //load our resources
        ui.loadXML("data/xmls/ui.xml");
    }

    void free()
    {
        //unload
        ui.free();
    }
}
