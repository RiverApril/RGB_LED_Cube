
#include "../cubeCore/cubeCore.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

int main(){

    JoystickCore::start();
    LightCore::start();

    bool dirty = true;

    bool running = true;

    pos_t cursor = 0;

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


        LightCore::clearAll();
        LightCore::setLight(1, 0, 0, 0xFF0000);
        LightCore::setLight(0, 1, 0, 0x00FF00);
        LightCore::setLight(0, 0, 1, 0x0000FF);
        LightCore::setLight(cursor, 0xFFFFFF);
        LightCore::swapBuffers();

        if(JoystickCore::buttonPressed[JS_BUTTON_A]){cursor++; cursor=cursor&0x1FF;}
        if(JoystickCore::buttonPressed[JS_BUTTON_B]){cursor--; cursor=cursor&0x1FF;}
        if(JoystickCore::buttonPressed[JS_BUTTON_X]){cursor=0;}


    }

    JoystickCore::stop();
    LightCore::stop();

    return 0;
}
