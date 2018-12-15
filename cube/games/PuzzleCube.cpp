
#include "../cubeCore/cubeCore.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

color_t colorMap[13] = {0xFF0000, 0xFF9500, 0x0000FF, 0x00FF00, 0xFFFFFF, 0xFFFF00,
                        0x7F0000, 0x7F4500, 0x00007F, 0x007F00, 0x7F7F7F, 0x7F7F00, 0xFF00FF};
int selectedToSideMap[9] = {0, -1, 1, 2, -1, 3, 4, -1, 5};
int sideOrders[12] = {
    2, 4, 3, 5, //x
    0, 4, 1, 5, //y
    0, 2, 1, 3};//z

unsigned char puzzleData[6*9];
bool selected[9];
/*
selected order:
0: x-
1: x0
2: x+

3: y-
4: y0
5: y+

6: z-
7: z0
8: z+

*/

/*

axis order: x, y, z

side order:
0: x-
1: x+
2: y-
3: y+
4: z-
5: z+

square order:
012
345
678


rot top: +z
sides: -x, -y, +x, +y: 0, 2, 1, 3
squares: 012

rot left: -x
sides: -y, -z, +y, +z: 2, 4, 3, 5

*/

void resetPuzzle(unsigned char data[6*9]){
    for(pos_t i = 0; i < 9; i++){
        for(pos_t side = 0; side < 6; side++){
            data[side*9+i] = side;
        }
    }
}

//unsigned char rotCoord[12];
//int tick = 0;

void turn(unsigned char data[6*9], unsigned char section, bool reverse){
    unsigned char rotCoord[12];

    for(int i = 0; i < 4; i++){
        int side = sideOrders[(section/3)*4+i];
        for(int k = 0; k < 3; k++){
            int j = (
                ((section/3==0)&&(side==2||side==5))|| // when rotationg along x axis sides 2 and 5 must be addressed backwards
                ((section/3==1)&&(side==2||side==0||side==5))|| // along y axis, 2, 0, and 5
                ((section/3==2)&&(side==3||side==0)) // along z axis, 3 and 0
            )?(2-k):(k);
            if(section/3 == 0 || (section/3 == 1 && side/2==0)){ // 0, 3, 6 or 1, 4, 7 or 2, 5, 8
                rotCoord[i*3+j] = side*9+(k*3+(section%3));
            }else{ // 0, 1, 2 or 3, 4, 5 or 6, 7, 8
                rotCoord[i*3+j] = side*9+((section%3)*3+k);
            }
        }
    }

    unsigned char rotData[12];
    for(int i = 0; i < 12; i++){
        rotData[i] = data[rotCoord[i]];
    }
    for(int i = 0; i < 12; i++){
        data[rotCoord[(i+(reverse?3:9))%12]] = rotData[i];
    }

    //012
    //345
    //678

    if(section%3!=1){ // side, not middle slice, so spin the side squares
        unsigned char spinCoord[8] = {0, 1, 2, 5, 8, 7, 6, 3};
        unsigned char spinData[8];
        for(int i = 0; i < 8; i++){
            spinCoord[i] = spinCoord[i] + selectedToSideMap[section]*9;
            spinData[i] = data[spinCoord[i]];
            //data[spinCoord[i]] = 6;
        }
        for(int i = 0; i < 8; i++){
            data[spinCoord[(i+(reverse?2:6))%8]] = spinData[i];
        }
        
    }
    
}

int main(){

    JoystickCore::start();
    LightCore::start();

    bool dirty = true;

    bool running = true;

    const double BLINK_DELAY = 0.25;
    double blinkTimer = 0;
    bool blink = false;

    bool noneSelected = true;

    clock_t begin = clock();

    resetPuzzle(puzzleData);
    for(pos_t i = 0; i < 9; i++){
        selected[i] = false;
    }

    while(running){
        JoystickCore::processEvents();

        if(JoystickCore::buttonPressed[JS_BUTTON_HOME]){
            running = false;
            break;
        }

        clock_t end = clock();
        double delta = double(end - begin) / CLOCKS_PER_SEC;
        begin = end;

        blinkTimer += delta;
        if(blinkTimer >= BLINK_DELAY){
            blinkTimer -= BLINK_DELAY;
            blink = !blink;
            dirty = true;
            //puzzleData[rotCoord[tick%12]] = 12;
            //tick++;

        }

        if(dirty){
            LightCore::clearAll();
            for(pos_t sx = 0; sx < 3; sx++){
                for(pos_t sy = 0; sy < 3; sy++){
                    pos_t i = sy*3+sx;

                    bool in;

                    for(pos_t qx = 0; qx < 2; qx++){
                        for(pos_t qy = 0; qy < 2; qy++){
                            in = ((sx==0&&selected[3]) || (sx==1&&selected[4]) || (sx==2&&selected[5]))
                            || ((sy==0&&selected[6]) || (sy==1&&selected[7]) || (sy==2&&selected[8]));
                            //if(!in||((qx==qy)==blink)){
                                LightCore::setLight(0, 1+sx*2+qx, 1+sy*2+qy, colorMap[puzzleData[0*9+i]+((in||selected[0]||noneSelected)?0:6)]); // (x-) (0)
                                LightCore::setLight(7, 1+sx*2+qx, 1+sy*2+qy, colorMap[puzzleData[1*9+i]+((in||selected[2]||noneSelected)?0:6)]); // (x+) (1)
                            //}
                            in = ((sx==0&&selected[0]) || (sx==1&&selected[1]) || (sx==2&&selected[2]))
                            || ((sy==0&&selected[6]) || (sy==1&&selected[7]) || (sy==2&&selected[8]));
                            //if(!in||((qx==qy)==blink)){
                                LightCore::setLight(1+sx*2+qx, 0, 1+sy*2+qy, colorMap[puzzleData[2*9+i]+((in||selected[3]||noneSelected)?0:6)]); // (y-) (2)
                                LightCore::setLight(1+sx*2+qx, 7, 1+sy*2+qy, colorMap[puzzleData[3*9+i]+((in||selected[5]||noneSelected)?0:6)]); // (y+) (3)
                            //}
                            in = ((sx==0&&selected[0]) || (sx==1&&selected[1]) || (sx==2&&selected[2]))
                            || ((sy==0&&selected[3]) || (sy==1&&selected[4]) || (sy==2&&selected[5]));
                            //if(!in||((qx==qy)==blink)){
                                LightCore::setLight(1+sx*2+qx, 1+sy*2+qy, 0, colorMap[puzzleData[4*9+i]+((in||selected[6]||noneSelected)?0:6)]); // (z-) (4)
                                LightCore::setLight(1+sx*2+qx, 1+sy*2+qy, 7, colorMap[puzzleData[5*9+i]+((in||selected[8]||noneSelected)?0:6)]); // (z+) (5)
                            //}
                        }
                    }
                }
            }
            LightCore::swapBuffers();
            dirty = false;
        }

        int newSelected = -1;
        if(JoystickCore::buttonDown[JS_BUTTON_L2] || JoystickCore::buttonDown[JS_BUTTON_R2]){
            if(JoystickCore::buttonPressed[JS_BUTTON_LEFT]){
                turn(puzzleData, 6, true);
                turn(puzzleData, 7, true);
                turn(puzzleData, 8, true);
                dirty = true;
            }
            if(JoystickCore::buttonPressed[JS_BUTTON_RIGHT]){
                turn(puzzleData, 6, false);
                turn(puzzleData, 7, false);
                turn(puzzleData, 8, false);
                dirty = true;
            }
            if(JoystickCore::buttonPressed[JS_BUTTON_UP]){
                turn(puzzleData, 3, false);
                turn(puzzleData, 4, false);
                turn(puzzleData, 5, false);
                dirty = true;
            }
            if(JoystickCore::buttonPressed[JS_BUTTON_DOWN]){
                turn(puzzleData, 3, true);
                turn(puzzleData, 4, true);
                turn(puzzleData, 5, true);
                dirty = true;
            }
            if(JoystickCore::buttonPressed[JS_BUTTON_L1]){
                turn(puzzleData, 0, false);
                turn(puzzleData, 1, false);
                turn(puzzleData, 2, false);
                dirty = true;
            }
            if(JoystickCore::buttonPressed[JS_BUTTON_R1]){
                turn(puzzleData, 0, true);
                turn(puzzleData, 1, true);
                turn(puzzleData, 2, true);
                dirty = true;
            }
        }else{
            if(JoystickCore::buttonPressed[JS_BUTTON_LEFT]){newSelected = 0;}
            if(JoystickCore::buttonPressed[JS_BUTTON_RIGHT]){newSelected = 2;}
            if(JoystickCore::buttonPressed[JS_BUTTON_UP]){newSelected = 3;}
            if(JoystickCore::buttonPressed[JS_BUTTON_DOWN]){newSelected = 5;}
            if(JoystickCore::buttonPressed[JS_BUTTON_L1]){newSelected = 6;}
            if(JoystickCore::buttonPressed[JS_BUTTON_R1]){newSelected = 8;}
        }

        if(JoystickCore::buttonPressed[JS_BUTTON_X]){newSelected = 9;};
        if(JoystickCore::buttonPressed[JS_BUTTON_Y]){newSelected = 9;};

        if(JoystickCore::buttonPressed[JS_BUTTON_A]){
            for(int i = 0; i < 9; i++){
                if(selected[i]){
                    turn(puzzleData, i, false);
                }
            }
            dirty = true;
        }
        if(JoystickCore::buttonPressed[JS_BUTTON_B]){
            for(int i = 0; i < 9; i++){
                if(selected[i]){
                    turn(puzzleData, i, true);
                }
            }
            dirty = true;
        }

        if(JoystickCore::buttonDown[JS_BUTTON_L2] && JoystickCore::buttonDown[JS_BUTTON_R2] && JoystickCore::buttonDown[JS_BUTTON_SELECT]){
            turn(puzzleData, rand()%9, rand()%2==0);
            dirty = true;
            for(int i = 0; i < 9; i++){
                selected[i] = false;
            }
            noneSelected = true;
        }

        if(JoystickCore::buttonPressed[JS_BUTTON_START]){
            resetPuzzle(puzzleData);
            dirty = true;
        }

        if(newSelected != -1){
            if(newSelected == 9){
                newSelected = -1;
            }

            if(selected[newSelected]){
                if(newSelected%3==0)newSelected++;
                else if(newSelected%3==2)newSelected--;
            }else if(newSelected%3==0 && selected[newSelected+1]){
                newSelected = -1;
            }else if(newSelected%3==2 && selected[newSelected-1]){
                newSelected = -1;
            }
            dirty = true;
            blinkTimer = BLINK_DELAY/2;
            for(int i = 0; i < 9; i++){
                selected[i] = (i==newSelected);
            }
            noneSelected = (newSelected == -1);
        }

    }

    JoystickCore::stop();
    LightCore::stop();

    return 0;
}
