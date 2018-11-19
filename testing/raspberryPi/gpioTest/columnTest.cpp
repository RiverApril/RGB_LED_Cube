#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>

#define PIN_Serial 14
#define PIN_Clock 30
#define PIN_Latch 21

int layerPins[8] = {6, 10, 11, 31, 26, 27, 28, 29};

int colors[8] = {4, 6, 2, 3, 1, 5, 0, 7};

//4: red
//2: green
//1: blue

int row[8];

void sendSlice(int offset){
    for(int i = 0; i < 8; i++){
        row[i] = colors[(i+offset)%8];
    }
    digitalWrite(PIN_Latch, 0);
    for(int i = 0; i < 8; i++){
        digitalWrite(PIN_Clock, 0);
        digitalWrite(PIN_Serial, (row[i]&1)?1:0);
        digitalWrite(PIN_Clock, 1);
    }
    for(int i = 0; i < 8; i++){
        digitalWrite(PIN_Clock, 0);
        digitalWrite(PIN_Serial, (row[i]&2)?1:0);
        digitalWrite(PIN_Clock, 1);
    }
    for(int i = 0; i < 8; i++){
        digitalWrite(PIN_Clock, 0);
        digitalWrite(PIN_Serial, (row[i]&4)?1:0);
        digitalWrite(PIN_Clock, 1);
    }
    digitalWrite(PIN_Latch, 1);
}

int main() {
    if (wiringPiSetup () == -1)
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

    int i = 0;

    int offset = 0;

    while(1) {
        digitalWrite(layerPins[i], 0);
        offset++;
        sendSlice(offset);
        usleep(1000);
        digitalWrite(layerPins[i], 1);
        i++;
        if(i==8){
            i=0;
        }
    }

    return 0 ;
}
