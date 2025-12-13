#include "TailViewAircraftSimulator.h"
#include <iostream>

int main(int argc, char **argv)
{
    try {
        TailViewAircraftSimulator app;
        app.setup();
        app.mRoot->startRendering();
        app.shutdown();
    }
    catch (Ogre::Exception& e) {
        std::cerr << "An OGRE exception has occurred: " << e.getFullDescription().c_str() << std::endl;
        return 1;
    }

    return 0;
}