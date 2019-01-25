
#include "../core/cubeCore.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

int main(){

    JoystickCore::start();
    LightCore::start();

    bool dirty = true;

    bool running = true;

    clock_t begin = clock();

    while(running){
        JoystickCore::processEvents();

        if(JoystickCore::buttonPressed[JS_BUTTON_HOME]){
            running = false;
            break;
        }

        clock_t end = clock();
        double delta = double(end - begin) / CLOCKS_PER_SEC;
        begin = end;



    }

    JoystickCore::stop();
    LightCore::stop();

    return 0;
}
