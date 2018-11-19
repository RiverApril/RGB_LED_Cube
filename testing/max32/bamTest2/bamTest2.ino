const int redPin = 17;
const int greenPin = 16;
const int bluePin = 15;

const int potPin = A0;

const int sw1Pin = 2;
const int sw2Pin = 7;
const int sw3Pin = 8;

const int LDS[8] = {70, 71, 72, 73, 74, 75, 76, 77};


#include <IOShieldOled.h>
String displayString;
char displayCharArray[50];
String redBAMfill;


struct Color{
  byte r, g, b;
};

Color ledColor;

uint32_t freq;

uint32_t timer = 0;
uint32_t counter = 0;

int currentBAMPeriod = 1;
int currentBAMBit = 0;


void __attribute__((interrupt)) myISR(){ // Timer 3
  
  timer++;
  if(timer >= currentBAMPeriod){ // should never be >

    digitalWrite(LDS[currentBAMBit==0?7:currentBAMBit-1], LOW);
    digitalWrite(LDS[currentBAMBit], HIGH);

    digitalWrite(redPin, (ledColor.r>>currentBAMBit)&1);
    digitalWrite(greenPin, (ledColor.g>>currentBAMBit)&1);
    digitalWrite(bluePin, (ledColor.b>>currentBAMBit)&1);
    
    currentBAMBit++;
    currentBAMPeriod = 1<<currentBAMBit;
    if(currentBAMBit == 8){
      currentBAMBit = 0;
    }
    
    timer = 0;
  }

  counter++;
  
  clearIntFlag(_TIMER_3_IRQ);
  
}

void start_timer_3(uint32_t frequency, int prescale) {

#define CLOCK_FREQ 80000000 // max32 runs at 80 Mhz
#define T3CON_ENABLE_BIT 0x8000
#define T3CON_PRESCALER_BITS 0x0070
#define T3_SOURCE_INT 0

  uint32_t period;
  if(prescale == 7){
    period = 256;
  }else{
    period = 1 << prescale;
  }
  period = period * frequency;
  period = CLOCK_FREQ / period;

  T3CONCLR = T3CON_ENABLE_BIT;         // Turn the timer off
  T3CONCLR = T3CON_PRESCALER_BITS;
  int mask = prescale << 4;
  mask = mask | T3CON;
  T3CON = mask;
  TMR3 = 0;
  PR3 = period;
  T3CONSET = T3CON_ENABLE_BIT;
  
  setIntVector(_TIMER_3_VECTOR, myISR);
  setIntPriority(_TIMER_3_VECTOR, 4, 0);
  clearIntFlag(_TIMER_3_IRQ);
  setIntEnable(_TIMER_3_IRQ);
} 

void setup() {
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);

  pinMode(potPin, INPUT);

  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, LOW);
  digitalWrite(bluePin, LOW);

  for(int i = 0; i < 8; i++){
    pinMode(LDS[i], OUTPUT);
    digitalWrite(LDS[i], LOW);
  }

  ledColor.r = 0x00;
  ledColor.g = 0x00;
  ledColor.b = 0x00;

  IOShieldOled.begin();
  IOShieldOled.displayOn();

  freq = 100000;
  start_timer_3(freq, 1);
}

/* http://www.instructables.com/id/Timer-Interrupts-on-the-DP32/
Prescaler   Max frequency   Min frequency
0, 1:1          80 MHz          1222 Hz
1, 1:2          40 MHz          612 Hz
2, 1:4          20 MHz          306 Hz
3, 1:8          10 MHz          154 Hz
4, 1:16         5 MHz           78 Hz
5, 1:32         2.5 MHz         40 Hz
6, 1:64         1250 kHz        20 Hz
     no 1:128
7, 1:256        624 kHz         10 Hz
 */

void loop() {

  if(digitalRead(sw3Pin)){
    ledColor.r = analogRead(potPin)/4;
  }
  if(digitalRead(sw2Pin)){
    ledColor.g = analogRead(potPin)/4;
  }
  if(digitalRead(sw1Pin)){
    ledColor.b = analogRead(potPin)/4;
  }

  
  /*redBAMfill = "";
  for(int i = 0; i < 8; i++){
    digitalWrite(redPin, (ledColor.r>>i)&1);
    redBAMfill += (ledColor.r>>i)&1?"*":"_";
    digitalWrite(greenPin, (ledColor.g>>i)&1);
    digitalWrite(bluePin, (ledColor.b>>i)&1);
    delayMicroseconds((1<<i));
  }*/


  IOShieldOled.clearBuffer();
  
  IOShieldOled.setCursor(0,0);
  displayString = "R: ";
  displayString += ledColor.r;
  //displayString += " ";
  //displayString += redBAMfill;
  displayString.toCharArray(displayCharArray,50);
  IOShieldOled.putString(displayCharArray);
  
  IOShieldOled.setCursor(0,1);
  displayString = "G: ";
  displayString += ledColor.g;
  displayString.toCharArray(displayCharArray,50);
  IOShieldOled.putString(displayCharArray);
  
  IOShieldOled.setCursor(0,2);
  displayString = "B: ";
  displayString += ledColor.b;
  displayString.toCharArray(displayCharArray,50);
  IOShieldOled.putString(displayCharArray);
  
  IOShieldOled.setCursor(0,3);
  displayString = "Seconds: ";
  displayString += counter/freq;
  displayString.toCharArray(displayCharArray,50);
  IOShieldOled.putString(displayCharArray);
  
  IOShieldOled.updateDisplay();

}
