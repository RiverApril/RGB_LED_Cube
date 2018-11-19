#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>


int main() {
  printf("test%c",0xA);
  if (wiringPiSetup () == -1)
    exit (1) ;

  printf("init%c",0xA);

  int pins[8] = {6, 10, 11, 31, 26, 27, 28, 29};

  for(int i = 0; i < 8; i++){
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], 1);
  }

  int i = 0;

  while(1) {
    //printf("pin %d%c", pins[i],0xA);
    digitalWrite(pins[i], 0);
    usleep(1000);
    digitalWrite(pins[i], 1);
    i++;
    if(i==8)i=0;
  }

  return 0 ;
}
