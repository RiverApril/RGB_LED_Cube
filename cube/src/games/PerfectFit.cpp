
#include "../core/cubeCore.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define min(a, b) (((a)<(b))?(a):(b))
#define max(a, b) (((a)>(b))?(a):(b))

color_t wallColor = 0xFFFF00;
color_t shapeColor = 0x0000FF;
color_t hitColor = 0xFF0000;
color_t fitColor = 0x00FF00;
color_t fitShapeColor = 0x00FFFF;

int shapeX;
int shapeY;
int shapeRot;

int wallPos;
double wallTimer;
double wallDelay = 0.3;
#define WALL_DELAY_FORMULA(x) (0.3-0.01*(x));

bool shape[8*8];
bool shapeTrans[8*8];
bool wall[8*8];

int level;
bool gameOver;
double gameOverTimer;
const double GAME_OVER_TIME = 3;

bool dirty = true;


// for degrees 0, 90, 180, 270
int cosine[4] = {1, 0, -1, 0};
int sine[4] = {0, 1, 0, -1};


int shapeCount(bool s[8*8]){
    int count = 0;
    for(pos_t i = 0; i < 8*8; i++){
        count += s[i];
    }
    return count;
}

void getCenter(bool s[8*8], double &cx, double &cy){
    int xMax = 0;
    int xMin = 7;
    int yMax = 0;
    int yMin = 7;
    for(pos_t i = 0; i < 8*8; i++){
        if(s[i]){
            xMax = max(i%8, xMax);
            xMin = min(i%8, xMin);
            yMax = max(i/8, yMax);
            yMin = min(i/8, yMin);
        }
    }
    cx = (xMax+xMin)/2.0;
    cy = (yMax+yMin)/2.0;
    printf("x: %d, %f, %d    y: %d, %f, %d\n", xMin, cx, xMax, yMin, cy, yMax);
}

void transformShape(bool trans[8*8], bool original[8*8], int xShift, int yShift, int rot){
    // first rotate about center
    // then translate

    // rot matrix:
    // cos, -sin
    // sin, cos
    double cx, cy;
    getCenter(original, cx, cy);
    rot = rot&3;
    printf("rot: %d\n", rot);
    for(pos_t i = 0; i < 8*8; i++){
        trans[i] = false;
    }
    for(pos_t i = 0; i < 8*8; i++){
        double x = i%8;
        double y = i/8;
        int newX = (int)((cosine[rot]*(x-cx) - sine[rot]*(y-cy)) + cx + xShift);
        int newY = (int)((sine[rot]*(x-cx) + cosine[rot]*(y-cy)) + cy + yShift);
        if(newX >= 0 && newY >= 0 && newX < 8 && newY < 8){
            trans[newX+newY*8] = original[i];
        }
    }
}

void centerShape(bool out[8*8], bool in[8*8]){
    double cx, cy;
    getCenter(in, cx, cy);
    int shiftX = (int)(3.5 - cx);
    int shiftY = (int)(3.5 - cy);
    printf("shift: %d, %d\n", shiftX, shiftY);
    transformShape(out, in, shiftX, shiftY, 0);
}

void resetForNextLevel(){

    shapeX = 0;
    shapeY = 0;
    shapeRot = 0;

    wallDelay = WALL_DELAY_FORMULA(level);

    wallPos = 0;
    wallTimer = -wallDelay; // a little starting delay

    for(pos_t i = 0; i < 8*8; i++){
        shape[i] = false;
    }

    // clever shape generator here
    // for now:
    int qty = round(2*log(level+1.5)+1);//(level/2)+3;
    shape[3+(rand()%2)+(3+(rand()%2))*8] = true;
    for(int i = 1; i < qty; i++){
        int nx, ny;
        bool adjacent;
        do{
            nx = rand()%6+1;
            ny = rand()%6+1;
            adjacent = false;
            adjacent |= shape[(nx+1)+(ny)*8];
            adjacent |= shape[(nx-1)+(ny)*8];
            adjacent |= shape[(nx)+(ny+1)*8];
            adjacent |= shape[(nx)+(ny-1)*8];
            adjacent &= !shape[(nx)+(ny)*8];
        }while(!adjacent);
        shape[nx + ny*8] = true;
    }
    centerShape(shapeTrans, shape);
    for(pos_t i = 0; i < 8*8; i++){
        shape[i] = shapeTrans[i];
    }

    do{
        transformShape(shapeTrans, shape, rand()%5-2, rand()%5-2, rand()%4);
    }while(shapeCount(shape) != shapeCount(shapeTrans)); // ensure cutout is on screen

    for(pos_t i = 0; i < 8*8; i++){
        wall[i] = !shapeTrans[i];
    }

    transformShape(shapeTrans, shape, shapeX, shapeY, shapeRot);
    
    dirty = true;
}

void resetGame(){
    level = 0;
    gameOver = 0;
    gameOverTimer = 0;
    resetForNextLevel();
}

int main(){

    srand(time(NULL));

    resetGame();

    JoystickCore::start();
    LightCore::start();

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


        if(dirty){
            dirty = false;
            LightCore::clearAll();

            if(gameOver && gameOverTimer <= 0){
                // show score
            }else{
                for(pos_t i = 0; i < 8*8; i++){
                    if(wallPos == 7){
                        bool hit = shapeTrans[i] && wall[i];
                        LightCore::setLight(0, i%8, i/8, wall[i]?(shapeTrans[i]?hitColor:fitColor):(shapeTrans[i]?fitShapeColor:0));
                        if(hit){
                            gameOverTimer = GAME_OVER_TIME;
                            gameOver = true;
                        }
                    }else{
                        LightCore::setLight(0, i%8, i/8, shapeTrans[i]?shapeColor:0);
                        LightCore::setLight(7-wallPos, i%8, i/8, wall[i]?wallColor:0);
                    }
                }
            }
            LightCore::swapBuffers();
        }

        if(gameOver){
            if(JoystickCore::buttonPressed[JS_BUTTON_START]){
                resetGame();
            }
        }


        wallTimer += delta;

        if(gameOver){
            if(gameOverTimer >= 0){
                gameOverTimer -= delta;
            }
        }else{
            if(wallTimer >= wallDelay){
                wallTimer -= wallDelay;
                if(wallPos == 7){
                    level++;
                    resetForNextLevel();
                }else{
                    wallPos++;
                    dirty = true;
                }
            }

            int shapeMoveX = 0;
            int shapeMoveY = 0;
            int shapeMoveRot = 0;
            if(JoystickCore::buttonPressed[JS_BUTTON_LEFT]){shapeMoveX = -1;}
            if(JoystickCore::buttonPressed[JS_BUTTON_RIGHT]){shapeMoveX = 1;}
            if(JoystickCore::buttonPressed[JS_BUTTON_UP]){shapeMoveY = 1;}
            if(JoystickCore::buttonPressed[JS_BUTTON_DOWN]){shapeMoveY = -1;}
            if(JoystickCore::buttonPressed[JS_BUTTON_L1]){shapeMoveRot = 1;}
            if(JoystickCore::buttonPressed[JS_BUTTON_R1]){shapeMoveRot = -1;}
            if(wallPos != 7 && (shapeMoveX!=0 || shapeMoveY!=0 || shapeMoveRot!=0)){
                int actualCount = shapeCount(shape);
                transformShape(shapeTrans, shape, shapeX+shapeMoveX, shapeY+shapeMoveY, shapeRot+shapeMoveRot);
                int trasCount = shapeCount(shapeTrans);
                if(trasCount == actualCount){
                    shapeX += shapeMoveX;
                    shapeY += shapeMoveY;
                    shapeRot += shapeMoveRot;
                }else{
                    transformShape(shapeTrans, shape, shapeX, shapeY, shapeRot);
                }
                dirty = true;
            }
        }
        



    }

    JoystickCore::stop();
    LightCore::stop();

    return 0;
}
