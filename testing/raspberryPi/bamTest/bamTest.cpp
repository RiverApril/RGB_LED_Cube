#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <cstdio>
#include <ctime>
#include <pthread.h>

#define PIN_Serial 14
#define PIN_Clock 30
#define PIN_Latch 21
int layerPins[8] = {6, 10, 11, 31, 26, 27, 28, 29};



const unsigned char lights[360]={
  0,   0,   0,   0,   0,   1,   1,   2, 
  2,   3,   4,   5,   6,   7,   8,   9, 
 11,  12,  13,  15,  17,  18,  20,  22, 
 24,  26,  28,  30,  32,  35,  37,  39, 
 42,  44,  47,  49,  52,  55,  58,  60, 
 63,  66,  69,  72,  75,  78,  81,  85, 
 88,  91,  94,  97, 101, 104, 107, 111, 
114, 117, 121, 124, 127, 131, 134, 137, 
141, 144, 147, 150, 154, 157, 160, 163, 
167, 170, 173, 176, 179, 182, 185, 188, 
191, 194, 197, 200, 202, 205, 208, 210, 
213, 215, 217, 220, 222, 224, 226, 229, 
231, 232, 234, 236, 238, 239, 241, 242, 
244, 245, 246, 248, 249, 250, 251, 251, 
252, 253, 253, 254, 254, 255, 255, 255, 
255, 255, 255, 255, 254, 254, 253, 253, 
252, 251, 251, 250, 249, 248, 246, 245, 
244, 242, 241, 239, 238, 236, 234, 232, 
231, 229, 226, 224, 222, 220, 217, 215, 
213, 210, 208, 205, 202, 200, 197, 194, 
191, 188, 185, 182, 179, 176, 173, 170, 
167, 163, 160, 157, 154, 150, 147, 144, 
141, 137, 134, 131, 127, 124, 121, 117, 
114, 111, 107, 104, 101,  97,  94,  91, 
 88,  85,  81,  78,  75,  72,  69,  66, 
 63,  60,  58,  55,  52,  49,  47,  44, 
 42,  39,  37,  35,  32,  30,  28,  26, 
 24,  22,  20,  18,  17,  15,  13,  12, 
 11,   9,   8,   7,   6,   5,   4,   3, 
  2,   2,   1,   1,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0, 
  0,   0,   0,   0,   0,   0,   0,   0};


unsigned int rainbow[360];

unsigned int buffer1[512];
unsigned int buffer2[512];

unsigned int** frontBuffer;
unsigned int** backBuffer;

// 255 max noramlly with 24 bit colors
// example with 15 max:
// ########.......  8|7
//         ####...  4|3
//             ##.  2|1
//               #  1|0

//     8       4   2 1
//  1: ..............#
//  2: ............##.
//  3: ............###
//  4: ........####...
//  5: ........####..#
//  6: ........######.
//  7: ........#######
//  8: ########.......
//  9: ########......#
// 10: ########....##.
// 11: ########....###
// 12: ############...
// 13: ############..#
// 14: ##############.
// 15: ###############

void render(unsigned int* buffer, unsigned char layer, unsigned char bamMask){
    digitalWrite(PIN_Latch, 0);
    for(int i = 0; i < 8; i++){ // red
        digitalWrite(PIN_Clock, 0);
        digitalWrite(PIN_Serial, ((buffer[layer*8+i]&0xFF0000)>>16)&bamMask);
        digitalWrite(PIN_Clock, 1);
    }
    for(int i = 0; i < 8; i++){ // green
        digitalWrite(PIN_Clock, 0);
        digitalWrite(PIN_Serial, ((buffer[layer*8+i]&0x00FF00)>>8)&bamMask);
        digitalWrite(PIN_Clock, 1);
    }
    for(int i = 0; i < 8; i++){ // blue
        digitalWrite(PIN_Clock, 0);
        digitalWrite(PIN_Serial, ((buffer[layer*8+i]&0x0000FF))&bamMask);
        digitalWrite(PIN_Clock, 1);
    }
    digitalWrite(PIN_Latch, 1);
}

void swapBuffers(){
    unsigned int* temp = *frontBuffer;
    *frontBuffer = *backBuffer;
    *backBuffer = temp;
}

void *renderThreadStart(void* frontBuffer){

    const int minDelay = 1;

    unsigned char layer = 0;
    unsigned char bamMask = 128;

    //clock_t begin = clock();
    while(1) {
        digitalWrite(layerPins[layer], 0);
        render(*((unsigned int**)(frontBuffer)), layer, bamMask);
        usleep(minDelay * bamMask);
        digitalWrite(layerPins[layer], 1);

        layer++;
        if(layer==8){
            layer=0;
            // went through all layers
            bamMask = bamMask >> 1;
            if(bamMask == 0){
                bamMask = 128;

                // fully updated cube

                //clock_t end = clock();
                //double frameTime = double(end - begin) / CLOCKS_PER_SEC;
                //printf("Frame time: %f\n", frameTime);
                //begin = clock();
            }
        }
    }
}

int main() {
    if (wiringPiSetup() == -1)
        exit(1);


    for(int i = 0; i < 8; i++){
        pinMode(layerPins[i], OUTPUT);
        digitalWrite(layerPins[i], 1);
    }

    pinMode(PIN_Serial, OUTPUT);
    digitalWrite(PIN_Serial, 0);

    pinMode(PIN_Clock, OUTPUT);
    digitalWrite(PIN_Clock, 0);

    pinMode(PIN_Latch, OUTPUT);
    digitalWrite(PIN_Latch, 0);

    for(int i = 0; i < 360; i++){ 
        rainbow[i] = ((lights[(i+120)%360])<<16) | ((lights[i])<<8) | (lights[(i+240)%360]);
    }


    pthread_t renderThread;

    frontBuffer = new unsigned int*(&buffer1[0]);
    backBuffer = new unsigned int*(&buffer2[0]);

    pthread_create(&renderThread, 0, &renderThreadStart, (void*)frontBuffer);


    int j = 0;

    while(1){
        clock_t begin = clock();
        int i = 0;
        for(int x = 0; x < 8; x++){
            for(int y = 0; y < 8; y++){
                for(int z = 0; z < 8; z++){
                    (*backBuffer)[x*64+y*8+z] = rainbow[(((i++)+j)*4)%360];
                }
            }
        }
        j++;
        swapBuffers();
        usleep(100000);
        clock_t end = clock();
        double frameTime = double(end - begin) / CLOCKS_PER_SEC;
        printf("Frame time: %f\n", frameTime);
        begin = end;
    }


    

    return 0 ;
}
