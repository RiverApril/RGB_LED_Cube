
#include "../cubeCore/cubeCore.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define COLOR_1 0xFF0000
#define COLOR_2 0x0000FF
#define COLOR_BLINK_1 0xFF8080
#define COLOR_BLINK_2 0x8080FF

unsigned char marks[512];
unsigned char winnerMarks[512];

unsigned char checkWin(int x, int y, int z){
    unsigned char landColor = marks[COORD_8(x, y, z)];
    if(landColor == 0){
        return 0;
    }
    for(int xx = -1; xx <= 1; xx++){
        for(int yy = -1; yy <= 1; yy++){
            for(int zz = -1; zz <= 1; zz++){
                if(xx != 0 || yy != 0 || zz != 0){
                    int px = x;
                    int py = y;
                    int pz = z;
                    while(marks[COORD_8(px, py, pz)]==landColor && px >= 0 && px <= 7 && py >= 0 && py <= 7 && pz >= 0 && pz <= 7){
                        px -= xx;
                        py -= yy;
                        pz -= zz;
                    }
                    px += xx;
                    py += yy;
                    pz += zz;
                    int count = 1;
                    while(marks[COORD_8(px, py, pz)]==landColor && px >= 0 && px <= 7 && py >= 0 && py <= 7 && pz >= 0 && pz <= 7){
                        px += xx;
                        py += yy;
                        pz += zz;
                        count++;
                    }
                    px -= xx;
                    py -= yy;
                    pz -= zz;
                    count--;
                    if(count >= 4){
                        while(marks[COORD_8(px, py, pz)]==landColor && px >= 0 && px <= 7 && py >= 0 && py <= 7 && pz >= 0 && pz <= 7){
                            winnerMarks[COORD_8(px, py, pz)] = landColor;
                            px -= xx;
                            py -= yy;
                            pz -= zz;
                        }
                        return landColor;
                    }

                }
            }
        }
    }
    return 0;
}

int main(){

    JoystickCore::start();
    LightCore::start();

    pos_t cursorX = 0, cursorY = 0;

    const double BLINK_DELAY = 0.25;
    double blinkTimer = 0;
    bool blinkOn = false;

    bool dirty = true;

    bool turnP1 = true;

    bool running = true;

    unsigned char winner = 0;

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

        blinkTimer += delta;
        if(blinkTimer >= BLINK_DELAY){
            blinkTimer -= BLINK_DELAY;
            blinkOn = !blinkOn;
            dirty = true;
        }

        if(dirty){
            LightCore::clearAll();
            for(pos_t i = 0; i < 512; i++){

                if(winner){
                    if(!blinkOn && winnerMarks[i]){
                        continue;
                    }
                }

                if(!winner && blinkOn && X_COORD_8(i) == cursorX && Y_COORD_8(i) == cursorY && Z_COORD_8(i) == 7){
                    LightCore::setLight(i, turnP1?COLOR_BLINK_1:COLOR_BLINK_2);
                    continue;
                }
                
                if(marks[i] == 1){
                    LightCore::setLight(i, COLOR_1);
                }else if(marks[i] == 2){
                    LightCore::setLight(i, COLOR_2);
                }
            }
            LightCore::swapBuffers();
            dirty = false;
        }

        bool moved = false;
        if(winner == 0){

            if(JoystickCore::buttonPressed[JS_BUTTON_LEFT]) { cursorX = (cursorX-1)&7; moved = true;}
            if(JoystickCore::buttonPressed[JS_BUTTON_RIGHT]){ cursorX = (cursorX+1)&7; moved = true;}
            if(JoystickCore::buttonPressed[JS_BUTTON_UP])   { cursorY = (cursorY-1)&7; moved = true;}
            if(JoystickCore::buttonPressed[JS_BUTTON_DOWN]) { cursorY = (cursorY+1)&7; moved = true;}

            if(JoystickCore::buttonPressed[JS_BUTTON_A]){
                if(marks[COORD_8(cursorX, cursorY, 7)] == 0){
                    pos_t fallZ = 7;
                    while(fallZ>0 && marks[COORD_8(cursorX, cursorY, fallZ-1)] == 0){
                        fallZ--;
                    }
                    marks[COORD_8(cursorX, cursorY, fallZ)] = turnP1?1:2;
                    turnP1 = !turnP1;
                    dirty = true;
                    winner = checkWin(cursorX, cursorY, fallZ);
                }
            }

        }else{
            if(JoystickCore::buttonPressed[JS_BUTTON_START]){
                
                for(pos_t i = 0; i < 512; i++){
                    marks[i] = 0;
                    winnerMarks[i] = 0;
                }
                turnP1 = true;
                winner = 0;

            }
        }

        if(moved){
            dirty = true;
            blinkOn = true;
            blinkTimer = 0;
        }
    }

    JoystickCore::stop();
    LightCore::stop();

    return 0;
}
