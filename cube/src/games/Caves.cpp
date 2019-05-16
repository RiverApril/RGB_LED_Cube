
#include "../core/cubeCore.hpp"
#include "../core/perlin.hpp"
#include "../core/color.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <vector>

#include "Caves/World.hpp"
#include "Caves/Entity.hpp"

using namespace std;

const int buttonMove[4] = {JS_BUTTON_LEFT, JS_BUTTON_RIGHT, JS_BUTTON_UP, JS_BUTTON_DOWN};
const int xDif[4] = {-1, 1, 0, 0};
const int yDif[4] = {0, 0, -1, 1};

const int buttonStayStill = JS_BUTTON_L2;
const int buttonHit = JS_BUTTON_X;
const int buttonUse = JS_BUTTON_A;
const int buttonAimUp = JS_BUTTON_R1;
const int buttonAimDown = JS_BUTTON_L1;

const int buttonNextItem = JS_BUTTON_SELECT;


#define MOVE_DELAY (0.1)

World* world;

Player* player;

Pos3 cameraCenter;
int xFace = 1;
int yFace = 0;

#define AIM_BELOW 0
#define AIM_DOWN 1
#define AIM_FOOT 2
#define AIM_FACE 3
#define AIM_UP 4
#define AIM_ABOVE 5
#define AIM_MIN AIM_BELOW
#define AIM_MAX AIM_ABOVE

int aim = AIM_FACE;

double moveTimers[4];
double blinkTimer;
#define BLINK_TIMER_MAX (0.1)
#define BLINK_TIMER_MID (BLINK_TIMER_MAX/2.0)

#define CURSOR_ON_COLOR (0xFFFFFF)

Block inventoryBlock = torch;

double timeOfDay = 0;


double brightnessFix(double x){
    x-=1;
    return 1-x*x*x*x;
}


void initGame(){
    world = new World();

    player = new Player();
    player->pos.x = WORLD_X_SIZE/2;
    player->pos.y = WORLD_Y_SIZE/2;
    player->pos.z = WORLD_Z_SIZE-1;
    player->height = 2;
    player->color = 0x0000FF;
    world->entities.push_back(player);

    cameraCenter.x = player->pos.x - 4;
    cameraCenter.y = player->pos.y - 4;
    cameraCenter.z = player->pos.z - 4;

    double seed = rand()/7177.0;

    for(int x = 0; x < WORLD_X_SIZE; x++){
        for(int y = 0; y < WORLD_Y_SIZE; y++){

            double r = noise(x/37.0, y/37.0, seed);
            int level = 32 + (r)*20;

            for(int z = 0; z < WORLD_Z_SIZE; z++){

                Block toSet = air;
                if(z < level-2){
                    toSet = stone;
                } else if(z < level){
                    toSet = dirt;
                } else if(z == level){
                    toSet = grass;
                }

                if(noise(x/97.0, y/97.0, z/97.0, seed) < 0.3){
                    toSet = air;
                }

                world->blocks[WORLD_INDEX(x, y, z)] = toSet;
                world->resetBlockLight(x, y, z, 0);
            }

            world->resetSkyLight(x, y);
        }
    }

    for(int x = 0; x < WORLD_X_SIZE; x++){
        for(int y = 0; y < WORLD_Y_SIZE; y++){
            for(int z = 0; z < WORLD_Z_SIZE; z++){
                world->updateLight(x, y, z);
            }
        }
    }
}

void movePlayer(int x, int y){
    if(!isSolid(world->getBlock(player->pos + Pos3(x, y, -1))) && !isSolid(world->getBlock(player->pos + Pos3(x, y, 0)))){
        player->pos = player->pos + Pos3(x, y, -1);
    }else if(!isSolid(world->getBlock(player->pos + Pos3(x, y, 0))) && !isSolid(world->getBlock(player->pos + Pos3(x, y, 1)))){
        player->pos = player->pos + Pos3(x, y, 0);
    }else if(!isSolid(world->getBlock(player->pos + Pos3(x, y, 1))) && !isSolid(world->getBlock(player->pos + Pos3(x, y, 2)))){
        player->pos = player->pos + Pos3(x, y, 1);
    }
}

void updateGame(double delta){

    // Daytime

    timeOfDay += delta*0.01;


    // Controls:

    for(int i = 0; i < 4; i++){
        if(JoystickCore::buttonDown[buttonMove[i]]){
            if(moveTimers[i] >= MOVE_DELAY){
                moveTimers[i] = 0.0;
            }
            if(moveTimers[i] == 0.0 && !(JoystickCore::buttonDown[buttonStayStill])){
                movePlayer(xDif[i], yDif[i]);
            }
            moveTimers[i] += delta;
        }else if(moveTimers[i] < MOVE_DELAY){
            moveTimers[i] += delta;
        }
    }

    int nxFace = JoystickCore::buttonDown[buttonMove[1]] - JoystickCore::buttonDown[buttonMove[0]];
    int nyFace = JoystickCore::buttonDown[buttonMove[3]] - JoystickCore::buttonDown[buttonMove[2]];
    
    if(nxFace != 0 || nyFace != 0){
        xFace = nxFace;
        yFace = nyFace;
    }

    if(JoystickCore::buttonPressed[buttonAimUp]){
        aim += 1;
        if(aim > AIM_MAX){
            aim = AIM_MAX;
        }
    }
    if(JoystickCore::buttonPressed[buttonAimDown]){
        aim -= 1;
        if(aim < AIM_MIN){
            aim = AIM_MIN;
        }
    }

    int aimX = 0;
    int aimY = 0;
    int aimZ = 0;

    switch(aim){
        case AIM_ABOVE:
        case AIM_UP: aimZ += 2; break;

        case AIM_FACE: aimZ += 1; break;

        case AIM_DOWN:
        case AIM_BELOW: aimZ -= 1; break;
        default: break;
    }
    switch(aim){
        case AIM_UP:
        case AIM_FACE:
        case AIM_FOOT:
        case AIM_DOWN: aimX += xFace; aimY += yFace; break;
        default: break;
    }

    if(JoystickCore::buttonPressed[buttonHit]){
        world->setBlock(player->pos + Pos3(aimX, aimY, aimZ), air);
    }

    if(JoystickCore::buttonPressed[buttonUse]){
        world->setBlock(player->pos + Pos3(aimX, aimY, aimZ), inventoryBlock);
    }

    if(JoystickCore::buttonPressed[buttonNextItem]){
        inventoryBlock = (Block)(inventoryBlock+1);
        if(inventoryBlock >= BLOCK_COUNT){
            inventoryBlock = (Block)1;
        }
    }

    // Camera:

    if(player->pos.x < cameraCenter.x + 2){
        cameraCenter.x--;
    }
    if(player->pos.x > cameraCenter.x + 5){
        cameraCenter.x++;
    }
    if(player->pos.y < cameraCenter.y + 2){
        cameraCenter.y--;
    }
    if(player->pos.y > cameraCenter.y + 5){
        cameraCenter.y++;
    }
    cameraCenter.z = player->pos.z - 4;

    //printf("Player pos: %d, %d, %d\n", player->pos.x, player->pos.y, player->pos.z);


    // Rendering:
    
    //LightCore::clearAll();
    for(int i = 0; i < 8; i++){
        for(int j = 0; j < 8; j++){
            for(int k = 0; k < 8; k++){
                int x = cameraCenter.x + i;
                int y = cameraCenter.y + j;
                int z = cameraCenter.z + k;
                Block block = world->getBlock(x, y, z);
                unsigned char light = world->getLight(x, y, z, timeOfDay);
                LightCore::setLight(i, j, k, multiplyColor(getBlockColor(block), brightnessFix(light/15.0)));
                //LightCore::setLight(i, j, k, getBlockColor(block));
            }
        }
    }
    //printf("Entity Count: %lu\n", world->entities.size());
    for(Entity* e : world->entities){
        e->update(world, delta);
        for(int h = 0; h < e->height; h++){
            int i = e->pos.x-cameraCenter.x;
            int j = e->pos.y-cameraCenter.y;
            int k = e->pos.z+h-cameraCenter.z;
            if(i >= 0 && j >= 0 && k >= 0 && i < 8 && j < 8 && k < 8){
                LightCore::setLight(i, j, k, e->color);
            }
        }
    }

    blinkTimer += delta;
    if(blinkTimer >= BLINK_TIMER_MAX){
        blinkTimer -= BLINK_TIMER_MAX;
    }
    int i = player->pos.x-cameraCenter.x + aimX;
    int j = player->pos.y-cameraCenter.y + aimY;
    int k = player->pos.z-cameraCenter.z + aimZ;

    if(i >= 0 && j >= 0 && k >= 0 && i < 8 && j < 8 && k < 8){
        //if(blinkTimer > BLINK_TIMER_MID){
            LightCore::setLight(i, j, k, CURSOR_ON_COLOR);
        //}
    }

    LightCore::swapBuffers();
}

int main(){

    srand(time(NULL));

    JoystickCore::start();
    LightCore::start();

    bool dirty = true;

    bool running = true;

    clock_t begin = clock();

    blinkTimer = 0.0;

    initGame();

    for(int i = 0; i < 4; i++){
        moveTimers[i] = 0.0;
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

        updateGame(delta);

    }

    JoystickCore::stop();
    LightCore::stop();

    return 0;
}
