
#include "../cubeCore/cubeCore.hpp"
#include "../cubeCore/perlin.hpp"
#include "../cubeCore/color.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

double starGrid[512];


int main(){

    JoystickCore::start();
    LightCore::start();

    bool dirty = true;

    bool running = true;

    clock_t begin = clock();

    int effectIndex = 0;
    const int effectCount = 3;

    for(pos_t i = 0; i < 512; i++){
        starGrid[i] = 0.0;
    }
    double twingleTimer = 0;

    double undulateTime = 0;
    double undulateSpeed = 1;
    double noiseScale = 0.125;
    double maxPerlin = -1;
    double minPerlin = 1;

    srand(time(0));

    while(running){
        JoystickCore::processEvents();

        if(JoystickCore::buttonPressed[JS_BUTTON_SELECT]){
            running = false;
            break;
        }

        clock_t end = clock();
        double delta = double(end - begin) / CLOCKS_PER_SEC;
        begin = end;

        if(JoystickCore::buttonPressed[JS_BUTTON_LEFT]){
            effectIndex = effectIndex==0?effectCount-1:effectIndex-1;
        }

        if(JoystickCore::buttonPressed[JS_BUTTON_RIGHT]){
            effectIndex = (effectIndex+1)%effectCount;
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
            case 2:{
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
        }


    }

    JoystickCore::stop();
    LightCore::stop();

    return 0;
}
