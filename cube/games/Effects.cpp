
#include "../cubeCore/cubeCore.hpp"
#include "../cubeCore/perlin.hpp"
#include "../cubeCore/color.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

double starGrid[512];
unsigned char pipeGrid[512];


int main(){

    JoystickCore::start();
    LightCore::start();

    bool dirty = true;

    bool running = true;

    clock_t begin = clock();

    int effectIndex = 2;
    const int effectCount = 4;

    double twingleTimer = 0;

    double undulateTime = 0;
    double undulateSpeed = 1;
    double noiseScale = 0.125;
    double maxPerlin = -1;
    double minPerlin = 1;

    const int PIPES = 4;
    pos_t pipeHeads[PIPES];
    unsigned char pipeDir[PIPES];
    const color_t colorMap[PIPES+1] = {0x000000, 0xFF0000, 0x00FF00, 0x0000FF, 0xFF00FF};
    double pipeTimer = 0;

    bool reset = true;

    srand(time(0));

    while(running){
        JoystickCore::processEvents();

        if(JoystickCore::buttonPressed[JS_BUTTON_HOME]){
            running = false;
            break;
        }

        clock_t end = clock();
        double delta = double(end - begin) / CLOCKS_PER_SEC;
        begin = end;

        if(JoystickCore::buttonPressed[JS_BUTTON_LEFT]){
            effectIndex = effectIndex==0?effectCount-1:effectIndex-1;
            reset = true;
        }

        if(JoystickCore::buttonPressed[JS_BUTTON_RIGHT]){
            effectIndex = (effectIndex+1)%effectCount;
            reset = true;
        }

        if(reset){
            reset = false;
            for(pos_t i = 0; i < 512; i++){
                starGrid[i] = 0.0;
                pipeGrid[i] = 0;
            }
            for(int i = 0; i < PIPES; i++){
                pipeHeads[i] = rand()%512;
                pipeDir[i] = rand()%6;
            }
        }

        switch(effectIndex){
            case 0: // Twinkle Stars
            case 1:{ // Rain
                twingleTimer -= delta;
                if(twingleTimer <= 0){
                    twingleTimer = ((rand()%100)/100.0)*0.15 + 0.00;
                    if(effectIndex == 1){
                        starGrid[COORD_8(rand()%8, rand()%8, 7)] = 0.001;
                    }else{
                        starGrid[rand()%512] = 0.001;
                    }
                }
                LightCore::clearAll();
                for(pos_t i = 0; i < 512; i++){
                    if(starGrid[i] != 0){
                        if(starGrid[i] > 0){
                            if(starGrid[i] < 1){
                                starGrid[i] += delta*((effectIndex == 1)?2.0:1.5);
                            }else{
                                starGrid[i] = 0;
                            }
                            if(starGrid[i] > 1){
                                starGrid[i] = 1;
                            }
                        }
                        if(effectIndex == 1){
                            if(starGrid[i] > 0.1 && Z_COORD_8(i)!=0){
                                pos_t b = COORD_8(X_COORD_8(i), Y_COORD_8(i), Z_COORD_8(i)-1);
                                if(starGrid[b] == 0){
                                    starGrid[b] = 0.001;
                                }
                            }
                        }
                        double f = sin(3.14159265358979*starGrid[i]);
                        color_t c = (int)(f*0xFF);
                        LightCore::setLight(i, RGB(c, c, c));
                    }
                }
                LightCore::swapBuffers();
                break;
            }
            case 2:{ // 4D perlin rainbow
                undulateTime += delta * undulateSpeed;
                LightCore::clearAll();
                for(pos_t i = 0; i < 512; i++){
                    double perlin = noise(X_COORD_8(i)*noiseScale, Y_COORD_8(i)*noiseScale, Z_COORD_8(i)*noiseScale, undulateTime);
                    maxPerlin = perlin > maxPerlin ? perlin : maxPerlin;
                    minPerlin = perlin < minPerlin ? perlin : minPerlin;
                    perlin = (perlin-minPerlin)/(maxPerlin-minPerlin);
                    LightCore::setLight(i, hsv2rgb((perlin*360), 1, 1));
                }
                LightCore::swapBuffers();

                if(JoystickCore::buttonPressed[JS_BUTTON_A]){
                    undulateSpeed += 1;
                }
                if(JoystickCore::buttonPressed[JS_BUTTON_B]){
                    undulateSpeed -= 1;
                }

                if(JoystickCore::buttonPressed[JS_BUTTON_X]){
                    noiseScale *= 2;
                }
                if(JoystickCore::buttonPressed[JS_BUTTON_Y]){
                    noiseScale /= 2;
                }

                break;
            }
            case 3:{
                pipeTimer -= delta;
                if(pipeTimer <= 0){
                    pipeTimer = 0.05;
                    int tries = 0;
                    for(unsigned char i = 0; i < PIPES; i++){
                        if(tries > 50){
                            reset = true;
                            break;
                        }
                        pipeGrid[pipeHeads[i]] = i+1;
                        int dx = (pipeDir[i]==0)?-1:((pipeDir[i]==1)?1:0);
                        int dy = (pipeDir[i]==2)?-1:((pipeDir[i]==3)?1:0);
                        int dz = (pipeDir[i]==4)?-1:((pipeDir[i]==5)?1:0);
                        int nx = X_COORD_8(pipeHeads[i])+dx;
                        int ny = Y_COORD_8(pipeHeads[i])+dy;
                        int nz = Z_COORD_8(pipeHeads[i])+dz;
                        if(rand()%2 != 0){
                            if(nx>=0 && nx<=7 && ny>=0 && ny<=7 && nz>=0 && nz<=7){
                                pos_t next = COORD_8(nx, ny, nz);
                                if(pipeGrid[next] == 0){
                                    pipeHeads[i] = next;
                                    continue;
                                }
                            }
                        }else{
                            tries--;
                        }
                        pipeDir[i] = rand()%6;
                        tries++;
                        i--;
                        continue;
                    }
                }
                LightCore::clearAll();
                for(pos_t i = 0; i < 512; i++){
                    LightCore::setLight(i, colorMap[pipeGrid[i]]);
                }
                LightCore::swapBuffers();
                break;
            }
        }


    }

    JoystickCore::stop();
    LightCore::stop();

    return 0;
}
