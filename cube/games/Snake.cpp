
#include "../cubeCore/cubeCore.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define COLOR_PELLET 0xFF00FF
#define START_DELAY 0.5

int pelletX, pelletY, pelletZ;
int snakeX, snakeY, snakeZ;
int vx, vy, vz;

#define INITAL_MOVE_DELAY 0.5
double moveDelay = INITAL_MOVE_DELAY;
const double moveDelaySubtractor = 0.00625;
const double moveDelayMin = 0.1;
double deathDelay = 0.05;
double timer = -START_DELAY;

int death = 0;

int snakeLength = 1;

pos_t snake[512];

void movePellet(){
    do{
        pelletX = rand()%8;
        pelletY = rand()%8;
        pelletZ = rand()%8;
    }while(snake[COORD_8(pelletX, pelletY, pelletZ)] != 0);
}

color_t getSnakeColor(pos_t n){
    unsigned char w = (unsigned char)(int)(-127.5*cos((2*PI/16)*(snakeLength-n))+127.5);
    if(death == 0 || snakeLength-n > death){
        return RGB(0, w, 0xFF-w); // blue ... green ... blue ...
    }else{
        return RGB(0xFF, w, 0); // red ... yellow ... red ...
    }
}

int main(){

    JoystickCore::start();
    LightCore::start();

    srand(time(0));

    for(pos_t i = 0; i < 512; i++){
        snake[i] = 0;
    }

    snakeX = 3;
    snakeY = 3;
    snakeZ = 3;

    vx = 1;
    vy = 0;
    vz = 0;

    movePellet();

    bool dirty = true;

    bool running = true;

    clock_t begin = clock();

    while(running){
        JoystickCore::processEvents();

        if(JoystickCore::buttonPressed[JS_BUTTON_SELECT]){
            running = false;
            break;
        }

        clock_t end = clock();
        double delta = double(end - begin) / CLOCKS_PER_SEC;
        begin = end;

        timer += delta;

        if(death == 0){
            if(timer >= moveDelay){
                timer -= moveDelay;

                snakeX += vx;
                snakeY += vy;
                snakeZ += vz;

                if(snake[COORD_8(snakeX, snakeY, snakeZ)] > 0 || snakeX < 0 || snakeX >= 8 || snakeY < 0 || snakeY >= 8 || snakeZ < 0 || snakeZ >= 8){
                    death = 1;
                }else{

                    for(pos_t i = 0; i < 512; i++){
                        if(snake[i] > 0){
                            snake[i]--;
                        }
                    }

                    snake[COORD_8(snakeX, snakeY, snakeZ)] = snakeLength;

                    if(pelletX == snakeX && pelletY == snakeY && pelletZ == snakeZ){
                        snakeLength++;
                        for(pos_t i = 0; i < 512; i++){
                            if(snake[i] > 0){
                                snake[i]++;
                            }
                        }
                        movePellet();
                        moveDelay -= moveDelaySubtractor;
                        if(moveDelay < moveDelayMin){
                            moveDelay = moveDelayMin;
                        }
                    }
                }

                dirty = true;
            }
        }else{
            if(timer >= deathDelay){
                timer -= deathDelay;

                death++;

                dirty = true;
            }
        }

        if(dirty){
            LightCore::clearAll();
            
            for(pos_t i = 0; i < 512; i++){
                if(X_COORD_8(i) == pelletX && Y_COORD_8(i) == pelletY && Z_COORD_8(i) == pelletZ){
                    LightCore::setLight(i, COLOR_PELLET);
                }else if(snake[i] != 0){
                    LightCore::setLight(i, getSnakeColor(snake[i]));
                }
            }

            LightCore::swapBuffers();
            dirty = false;
        }
        
        if(JoystickCore::buttonPressed[JS_BUTTON_LEFT]) { vx = -1; vy = 0; vz = 0; }
        if(JoystickCore::buttonPressed[JS_BUTTON_RIGHT]){ vx = 1; vy = 0; vz = 0; }
        if(JoystickCore::buttonPressed[JS_BUTTON_UP])   { vx = 0; vy = -1; vz = 0; }
        if(JoystickCore::buttonPressed[JS_BUTTON_DOWN]) { vx = 0; vy = 1; vz = 0; }
        if(JoystickCore::buttonPressed[JS_BUTTON_L1])   { vx = 0; vy = 0; vz = -1; }
        if(JoystickCore::buttonPressed[JS_BUTTON_R1])   { vx = 0; vy = 0; vz = 1; }

        if(death){
            if(JoystickCore::buttonPressed[JS_BUTTON_START]){
                death = 0;
                timer = -START_DELAY;

                for(pos_t i = 0; i < 512; i++){
                    snake[i] = 0;
                }

                snakeX = 3;
                snakeY = 3;
                snakeZ = 3;

                vx = 1;
                vy = 0;
                vz = 0;

                snakeLength = 1;

                moveDelay = INITAL_MOVE_DELAY;

                movePellet();

                for(pos_t i = 0; i < 512; i++){
                    LightCore::setLight(i, getSnakeColor(snake[i]));
                }

            }
        }

    }

    JoystickCore::stop();
    LightCore::stop();

    return 0;
}
