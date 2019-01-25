
#include "../core/cubeCore.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <vector>

using namespace std;

struct point{
    int x, y, z;
};

const int FULL_BARRIER = 4;
const int BARRIER_Z = 1;
const int PLAYER_Z = 0;

int barriers[64];
int playerX = 2;
int playerY = 2;


vector<point> shots;

void reset(){
    for(int i = 0; i < 64; i++){
        barriers[i] = 0;
    }
    const int barrierStart[4] = {9, 13, 41, 45};
    for(int i = 0; i < 4; i++){
        barriers[barrierStart[i]] = FULL_BARRIER;
        barriers[barrierStart[i]+1] = FULL_BARRIER;
        barriers[barrierStart[i]+8] = FULL_BARRIER;
        barriers[barrierStart[i]+9] = FULL_BARRIER;
    }
}

int main(){

    JoystickCore::start();
    LightCore::start();

    bool dirty = true;

    bool running = true;

    reset();

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
