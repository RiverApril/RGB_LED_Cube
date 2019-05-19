
#include "core/cubeCore.hpp"

#include <stdio.h>

int main(){

    JoystickCore::start();
    LightCore::start();

    LightCore::clearAll();
    LightCore::swapBuffers();

    printf("Lights Turned Off\n");

    JoystickCore::stop();
    LightCore::stop();

    return 0;
}
