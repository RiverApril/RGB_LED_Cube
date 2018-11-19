
#include "../cubeCore/cubeCore.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define COLOR_PELLET 0xFF00FF
#define START_DELAY 0.5

struct P3{
    int x, y, z;
};

P3 snakes[2];
P3 snakesV[2];

P3 pellets[2];


#define INITAL_MOVE_DELAY 0.5
double moveDelay = INITAL_MOVE_DELAY;
const double moveDelaySubtractor = 0.00625;
const double moveDelayMin = 0.1;
double deathDelay = 0.05;
double timer = -START_DELAY;

int death = 0;

int snakeLength[2];

int snake[512];

void movePellet(int i){
    do{
        pellets[i].x = rand()%8;
        pellets[i].y = rand()%8;
        pellets[i].z = rand()%8;
    }while(snake[COORD_8(pellets[i].x, pellets[i].y, pellets[i].z)] != 0);
}

color_t getSnakeColor(int n){
    bool p1 = true;
    if(n < 0){
        n = -n;
        p1 = false;
    }
    unsigned char w = (unsigned char)(int)(-127.5*cos((2*PI/16)*(snakeLength[p1?0:1]-n))+127.5);
    if(death == 0 || snakeLength[p1?0:1]-n > death){
        return p1
        ?RGB(0, w/2, 0xFF-w) // blue ... teal ... blue ...
        :RGB(w/2, 0xFF-w, 0); // green ... yellow ... green ...
    }else{
        return RGB(0xFF, w/2, 0); // red ... orange ... red ...
    }
}

int main(){

    JoystickCore::start();
    LightCore::start();

    srand(time(0));

    for(pos_t i = 0; i < 512; i++){
        snake[i] = 0;
    }

    
    snakes[0].x = 2;
    snakes[0].y = 2;
    snakes[0].z = 2;

    snakesV[0].x = 1;
    snakesV[0].y = 0;
    snakesV[0].z = 0;
    
    snakes[1].x = 5;
    snakes[1].y = 5;
    snakes[1].z = 5;

    snakesV[1].x = -1;
    snakesV[1].y = 0;
    snakesV[1].z = 0;

    snakeLength[0] = 1;
    snakeLength[1] = 1;

    movePellet(0);
    movePellet(1);

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

                for(pos_t i = 0; i < 512; i++){
                    if(snake[i] > 0){
                        snake[i]--;
                    }
                    if(snake[i] < 0){
                        snake[i]++;
                    }
                }

                for(unsigned char s = 0; s < 2; s++){

                    snakes[s].x += snakesV[s].x;
                    snakes[s].y += snakesV[s].y;
                    snakes[s].z += snakesV[s].z;

                    if(snake[COORD_8(snakes[s].x, snakes[s].y, snakes[s].z)] > 0 || snakes[s].x < 0 || snakes[s].x >= 8 || snakes[s].y < 0 || snakes[s].y >= 8 || snakes[s].z < 0 || snakes[s].z >= 8){
                        death = s?1:-1;
                    }else{

                        snake[COORD_8(snakes[s].x, snakes[s].y, snakes[s].z)] = snakeLength[s];

                        for(unsigned char p = 0; p < 2; p++){

                            if(pellets[p].x == snakes[s].x && pellets[p].y == snakes[s].y && pellets[p].z == snakes[s].z){
                                snakeLength[s]++;
                                for(pos_t i = 0; i < 512; i++){
                                    if(p == 0 && snake[i] > 0){
                                        snake[i]++;
                                    }
                                    if(p == 1 && snake[i] < 0){
                                        snake[i]--;
                                    }
                                }
                                movePellet(p);
                                moveDelay -= moveDelaySubtractor;
                                if(moveDelay < moveDelayMin){
                                    moveDelay = moveDelayMin;
                                }
                            }

                        }
                    }

                }

                dirty = true;
            }
        }else{
            if(timer >= deathDelay){
                timer -= deathDelay;

                death += death>0?1:-1;

                dirty = true;
            }
        }

        if(dirty){
            LightCore::clearAll();
            
            for(pos_t i = 0; i < 512; i++){
                if((X_COORD_8(i) == pellets[0].x && Y_COORD_8(i) == pellets[0].y && Z_COORD_8(i) == pellets[0].z) ||
                (X_COORD_8(i) == pellets[1].x && Y_COORD_8(i) == pellets[1].y && Z_COORD_8(i) == pellets[1].z)){
                    LightCore::setLight(i, COLOR_PELLET);
                }else if(snake[i] != 0){
                    LightCore::setLight(i, getSnakeColor(snake[i]));
                }
            }

            LightCore::swapBuffers();
            dirty = false;
        }
        
        if(JoystickCore::buttonPressed[JS_BUTTON_LEFT]) { snakesV[0].x = -1; snakesV[0].y = 0; snakesV[0].z = 0; }
        if(JoystickCore::buttonPressed[JS_BUTTON_RIGHT]){ snakesV[0].x = 1; snakesV[0].y = 0; snakesV[0].z = 0; }
        if(JoystickCore::buttonPressed[JS_BUTTON_UP])   { snakesV[0].x = 0; snakesV[0].y = -1; snakesV[0].z = 0; }
        if(JoystickCore::buttonPressed[JS_BUTTON_DOWN]) { snakesV[0].x = 0; snakesV[0].y = 1; snakesV[0].z = 0; }
        if(JoystickCore::buttonPressed[JS_BUTTON_L1])   { snakesV[0].x = 0; snakesV[0].y = 0; snakesV[0].z = 1; }
        if(JoystickCore::buttonPressed[JS_BUTTON_L2])   { snakesV[0].x = 0; snakesV[0].y = 0; snakesV[0].z = -1; }
        
        if(JoystickCore::buttonPressed[JS_BUTTON_X]) { snakesV[1].x = -1; snakesV[1].y = 0; snakesV[1].z = 0; }
        if(JoystickCore::buttonPressed[JS_BUTTON_B]) { snakesV[1].x = 1; snakesV[1].y = 0; snakesV[1].z = 0; }
        if(JoystickCore::buttonPressed[JS_BUTTON_Y]) { snakesV[1].x = 0; snakesV[1].y = -1; snakesV[1].z = 0; }
        if(JoystickCore::buttonPressed[JS_BUTTON_A]) { snakesV[1].x = 0; snakesV[1].y = 1; snakesV[1].z = 0; }
        if(JoystickCore::buttonPressed[JS_BUTTON_R1]){ snakesV[1].x = 0; snakesV[1].y = 0; snakesV[1].z = 1; }
        if(JoystickCore::buttonPressed[JS_BUTTON_R2]){ snakesV[1].x = 0; snakesV[1].y = 0; snakesV[1].z = -1; }

        if(death){
            if(JoystickCore::buttonPressed[JS_BUTTON_START]){
                death = 0;
                timer = -START_DELAY;

                for(pos_t i = 0; i < 512; i++){
                    snake[i] = 0;
                }

                snakes[0].x = 2;
                snakes[0].y = 2;
                snakes[0].z = 2;

                snakesV[0].x = 1;
                snakesV[0].y = 0;
                snakesV[0].z = 0;
                
                snakes[1].x = 5;
                snakes[1].y = 5;
                snakes[1].z = 5;

                snakesV[1].x = -1;
                snakesV[1].y = 0;
                snakesV[1].z = 0;

                snakeLength[0] = 1;
                snakeLength[1] = 1;

                moveDelay = INITAL_MOVE_DELAY;

                movePellet(0);
                movePellet(1);

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
