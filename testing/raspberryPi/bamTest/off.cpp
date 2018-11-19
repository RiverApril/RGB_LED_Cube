#include <wiringPi.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>


int main() {
  if (wiringPiSetup () == -1)
    exit (1) ;

  int pins[8] = {6, 10, 11, 31, 26, 27, 28, 29};

  for(int i = 0; i < 8; i++){
    pinMode(pins[i], OUTPUT);
    digitalWrite(pins[i], 1);
  }

  return 0 ;
}
