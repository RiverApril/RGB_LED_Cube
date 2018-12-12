
#include "../cubeCore/cubeCore.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define COLOR_1 0xFF0000
#define COLOR_2 0x0000FF
#define COLOR_FLASH_1 0xFFFFFF
#define COLOR_FLASH_2 0xFFFFFF
#define COLOR_BLINK_1 0xFF8080
#define COLOR_BLINK_2 0x8080FF

unsigned char marks[512];
//0: off
//1: p1 piece
//2: p2 piece
//3: p1 piece new (flash)
//4: p2 piece new (flash)

void setMarksForNewGame(){
    for(pos_t i = 0; i < 512; i++){
        marks[i] = 0;
    }
    marks[COORD_8(3, 3, 3)] = 1;
    marks[COORD_8(3, 3, 4)] = 2;
    marks[COORD_8(3, 4, 3)] = 2;
    marks[COORD_8(4, 3, 3)] = 2;
    marks[COORD_8(4, 4, 3)] = 1;
    marks[COORD_8(3, 4, 4)] = 1;
    marks[COORD_8(4, 3, 4)] = 1;
    marks[COORD_8(4, 4, 4)] = 2;
}

bool tryLine(unsigned char player, char x, char y, char z, char xs, char ys, char zs){
    unsigned char other = player==1?2:1;
    bool found = false;
    for(unsigned char i = 1; true; i++){
        char xx = x+xs*i;
        char yy = y+ys*i;
        char zz = z+zs*i;
        if(xx >= 0 && xx < 8 && yy >= 0 && yy < 8 && zz >= 0 && zz < 8){
            unsigned char here = marks[COORD_8(xx, yy, zz)];
            if(here == 3 || here == 4){ here -= 2; } // to see flash tiles also

            if(here == 0){
                return false;
            }else if(here == player){
                return found;
            }else if(here == other){
                found = true;
            }else{
                return false;
            }
        }else{
            return false;
        }
    }
}

bool fillLine(unsigned char player, char x, char y, char z, char xs, char ys, char zs){
    unsigned char other = player==1?2:1;
    bool success = false;
    if(tryLine(player, x, y, z, xs, ys, zs)){
        for(unsigned char i = 0; true; i++){
            char xx = x+xs*i;
            char yy = y+ys*i;
            char zz = z+zs*i;
            if(xx >= 0 && xx < 8 && yy >= 0 && yy < 8 && zz >= 0 && zz < 8){
                unsigned char here = marks[COORD_8(xx, yy, zz)];
                if(here == other || (i==0)){
                    marks[COORD_8(xx, yy, zz)] = player+2; // +2 for flash
                    success = true;
                }else{
                    break;
                }
            }else{
                break;
            }
        }
    }
    return success;
}

int main(){

    JoystickCore::start();
    LightCore::start();

    bool dirty = true;

    bool running = true;

    pos_t cursorX = 0, cursorY = 0, cursorZ = 0;

    unsigned char winner = 0;

    const double BLINK_DELAY = 0.25;
    double blinkTimer = 0;
    bool blinkOn = false;
    
    const double FLASH_DELAY = 0.25;
    double flashTimer = 0;

    bool turnP1 = true;

    bool playerCantPlay = false;

    setMarksForNewGame();

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

        if(flashTimer >= 0){
            flashTimer += delta;
            if(flashTimer >= FLASH_DELAY){
                flashTimer = -1;
                for(pos_t i = 0; i < 512; i++){
                    if(marks[i] == 3 || marks[i] == 4){
                        marks[i]-=2;
                    }
                }
                dirty = true;
            }
        }

        if(dirty){
            LightCore::clearAll();
            for(pos_t i = 0; i < 512; i++){
                if(winner){
                    if(!blinkOn && winner == marks[i]){
                        continue;
                    }
                }else if(blinkOn && X_COORD_8(i) == cursorX && Y_COORD_8(i) == cursorY && Z_COORD_8(i) == cursorZ){
                    LightCore::setLight(i, turnP1?COLOR_BLINK_1:COLOR_BLINK_2);
                    continue;
                }
                if(marks[i] == 1){
                    LightCore::setLight(i, COLOR_1);
                }else if(marks[i] == 2){
                    LightCore::setLight(i, COLOR_2);
                }else if(marks[i] == 3){
                    LightCore::setLight(i, COLOR_FLASH_1);
                }else if(marks[i] == 4){
                    LightCore::setLight(i, COLOR_FLASH_2);
                }
            }
            LightCore::swapBuffers();
            dirty = false;
        }

        if(!winner){

            bool moved = false;

            if(JoystickCore::buttonPressed[JS_BUTTON_LEFT]) { cursorX = (cursorX-1)&7; moved = true;}
            if(JoystickCore::buttonPressed[JS_BUTTON_RIGHT]){ cursorX = (cursorX+1)&7; moved = true;}
            if(JoystickCore::buttonPressed[JS_BUTTON_UP])   { cursorY = (cursorY-1)&7; moved = true;}
            if(JoystickCore::buttonPressed[JS_BUTTON_DOWN]) { cursorY = (cursorY+1)&7; moved = true;}
            if(JoystickCore::buttonPressed[JS_BUTTON_L1])   { cursorZ = (cursorZ-1)&7; moved = true;}
            if(JoystickCore::buttonPressed[JS_BUTTON_R1])   { cursorZ = (cursorZ+1)&7; moved = true;}
        
            if(moved){
                dirty = true;
                blinkOn = true;
                blinkTimer = 0;
            }


            int p1Count = 0;
            int p2Count = 0;
            bool canPlay = false;
            for(pos_t i = 0; i < 512; i++){
                if(marks[i] == 0){
                    char x = X_COORD_8(i);
                    char y = Y_COORD_8(i);
                    char z = Z_COORD_8(i);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, -1, -1, -1);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, -1, 1, -1);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, -1, -1, 1);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, -1, 1, 1);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, -1, -1, 0);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, -1, 0, -1);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, -1, 1, 0);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, -1, 0, 1);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, -1, 0, 0);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, 1, -1, -1);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, 1, 1, -1);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, 1, -1, 1);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, 1, 1, 1);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, 1, -1, 0);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, 1, 0, -1);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, 1, 1, 0);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, 1, 0, 1);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, 1, 0, 0);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, 0, -1, -1);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, 0, 1, -1);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, 0, -1, 1);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, 0, 1, 1);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, 0, -1, 0);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, 0, 0, -1);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, 0, 1, 0);
                    canPlay |= tryLine(turnP1?1:2, x, y, z, 0, 0, 1);
                }else{
                    if(marks[i]==1){
                        p1Count++;
                    }
                    if(marks[i]==2){
                        p2Count++;
                    }
                }
            }

            if(canPlay){
                playerCantPlay = false;
            }else{
                if(playerCantPlay){
                    flashTimer = -1;
                    if(p1Count > p2Count){
                        winner = 1;
                    }else if(p1Count < p2Count){
                        winner = 2;
                    }else{
                        winner = 3;
                    }
                }else{
                    playerCantPlay = true;
                    turnP1 = !turnP1;
                    // Skipping turn, flash as indicator
                    for(pos_t i = 0; i < 512; i++){
                        if(marks[i] == 1 || marks[i] == 2){
                            marks[i]+=2;
                        }
                    }
                    flashTimer = 0;
                }
            }

            if(JoystickCore::buttonPressed[JS_BUTTON_A]){
                if(marks[COORD_8(cursorX, cursorY, cursorZ)] == 0){
                    bool placed = false;
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ, -1, -1, -1)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ, -1,  1, -1)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ, -1, -1,  1)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ, -1,  1,  1)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ, -1, -1,  0)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ, -1,  0, -1)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ, -1,  1,  0)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ, -1,  0,  1)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ, -1,  0,  0)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ,  1, -1, -1)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ,  1,  1, -1)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ,  1, -1,  1)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ,  1,  1,  1)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ,  1, -1,  0)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ,  1,  0, -1)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ,  1,  1,  0)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ,  1,  0,  1)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ,  1,  0,  0)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ,  0, -1, -1)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ,  0,  1, -1)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ,  0, -1,  1)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ,  0,  1,  1)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ,  0, -1,  0)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ,  0,  0, -1)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ,  0,  1,  0)){ placed=true; }
                    if(fillLine(turnP1?1:2, cursorX, cursorY, cursorZ,  0,  0,  1)){ placed=true; }
                    if(placed){
                        turnP1 = !turnP1;
                        flashTimer = 0;
                    }
                }
            }

        }else{
            if(JoystickCore::buttonPressed[JS_BUTTON_START]){
                setMarksForNewGame();
                turnP1 = true;
                winner = 0;
            }
        }


    }

    JoystickCore::stop();
    LightCore::stop();

    return 0;
}
