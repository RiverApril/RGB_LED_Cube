
const int clockPin = 19;
const int latchPin = 20;
//const int redSerialPin = 17;
//const int greenSerialPin = 16;
//const int blueSerialPin = 15;
const int serialPin = 15;

const int layerSimPin = 17;
const int layerManualPin = 16;


const int potPin = A0;


const int btn1Pin = 4;
bool lastButton = 0;

const int sw1Pin = 2;
const int sw2Pin = 7;
const int sw3Pin = 8;
const int sw4Pin = 79;

const int LDS[8] = {70, 71, 72, 73, 74, 75, 76, 77};


#include <IOShieldOled.h>
String displayString;
char displayCharArray[50];


struct Color{
  byte r, g, b;
};

Color ledColor;

Color ledColorBuf[8];

int colorAngle = 0;
Color rainbow[360];
const uint8_t lights[360]={
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


uint32_t freq;

uint32_t timer = 0;
uint32_t counter = 0;

int currentBAMPeriod = 1;
int currentBAMBit = 0;
int layer = 0;

int i;

void __attribute__((interrupt)) myISR(){ // Timer 3
  
  if(timer >= currentBAMPeriod){ // should never be >

    //digitalWrite(LDS[currentBAMBit==0?7:currentBAMBit-1], LOW);
    //digitalWrite(LDS[currentBAMBit], HIGH);


    digitalWrite(LDS[1], LOW);
    
    digitalWrite(latchPin, LOW);
    /*for(i = 7; i >= 0; i--){
      digitalWrite(clockPin, LOW);
      digitalWrite(redSerialPin, (ledColorBuf[i].r>>currentBAMBit)&1);
      digitalWrite(greenSerialPin, (ledColorBuf[i].g>>currentBAMBit)&1);
      digitalWrite(blueSerialPin, (ledColorBuf[i].b>>currentBAMBit)&1);
      digitalWrite(clockPin, HIGH);
    }*/
    /*
    for(i = 7; i >= 0; i--){
      digitalWrite(clockPin, LOW);
      digitalWrite(serialPin, (ledColorBuf[i].r>>currentBAMBit)&1);
      digitalWrite(clockPin, HIGH);
    }
    for(i = 7; i >= 0; i--){
      digitalWrite(clockPin, LOW);
      digitalWrite(serialPin, (ledColorBuf[i].g>>currentBAMBit)&1);
      digitalWrite(clockPin, HIGH);
    }
    for(i = 7; i >= 0; i--){
      digitalWrite(clockPin, LOW);
      digitalWrite(serialPin, (ledColorBuf[i].b>>currentBAMBit)&1);
      digitalWrite(clockPin, HIGH);
    }*/
    for(i = 7; i >= 0; i--){
      digitalWrite(clockPin, LOW);
      digitalWrite(serialPin, (ledColorBuf[i].r>>currentBAMBit)&1);
      digitalWrite(clockPin, HIGH);
      digitalWrite(clockPin, LOW);
      digitalWrite(serialPin, (ledColorBuf[i].g>>currentBAMBit)&1);
      digitalWrite(clockPin, HIGH);
      digitalWrite(clockPin, LOW);
      digitalWrite(serialPin, (ledColorBuf[i].b>>currentBAMBit)&1);
      digitalWrite(clockPin, HIGH);
    }
    digitalWrite(latchPin, HIGH);
    
    digitalWrite(LDS[1], HIGH);
    
    
    currentBAMBit++;
    currentBAMPeriod = 1<<currentBAMBit;
    if(currentBAMBit == 8){
      currentBAMBit = 0;
    }
    
    timer = 0;


  }


  digitalWrite(layerSimPin, HIGH);
  layer++;
  if(layer==8){
    layer = 0;
    timer++;
  }
  if(layer==0)digitalWrite(layerSimPin, LOW);

  counter++;
  
  clearIntFlag(_TIMER_3_IRQ);
  
}

Color makeColor(byte red, byte green, byte blue){
  Color c;
  c.r = red;
  c.g = green;
  c.b = blue;
  return c;
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
  //pinMode(redSerialPin, OUTPUT);
  //pinMode(greenSerialPin, OUTPUT);
  //pinMode(blueSerialPin, OUTPUT);
  pinMode(serialPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(latchPin, OUTPUT);
  pinMode(layerSimPin, OUTPUT);
  pinMode(layerManualPin, OUTPUT);

  
  
  pinMode(sw1Pin, INPUT);
  pinMode(sw2Pin, INPUT);
  pinMode(sw3Pin, INPUT);
  pinMode(sw4Pin, INPUT);
  pinMode(btn1Pin, INPUT);

  pinMode(potPin, INPUT);

  //digitalWrite(redSerialPin, LOW);
  //digitalWrite(greenSerialPin, LOW);
  //digitalWrite(blueSerialPin, LOW);
  digitalWrite(serialPin, LOW);
  digitalWrite(clockPin, LOW);
  digitalWrite(latchPin, LOW);

  digitalWrite(layerSimPin, HIGH);
  digitalWrite(layerManualPin, LOW);

  for(int i = 0; i < 8; i++){
    pinMode(LDS[i], OUTPUT);
    digitalWrite(LDS[i], LOW);
  }

  digitalWrite(LDS[0], HIGH);

  IOShieldOled.begin();
  IOShieldOled.displayOn();

  freq = 300000; // 300kHz
  start_timer_3(freq, 4); // Prescaler 4

  ledColor.r = 0x00;
  ledColor.g = 0x00;
  ledColor.b = 0x00;

  for(int i = 0; i < 360; i++){ 
    rainbow[i].r = lights[(i+120)%360];
    rainbow[i].g = lights[i];
    rainbow[i].b = lights[(i+240)%360];
  }

  ledColorBuf[0] = makeColor(0xFF, 0x00, 0x00);
  ledColorBuf[1] = makeColor(0xFF, 0xFF, 0x00);
  ledColorBuf[2] = makeColor(0x00, 0xFF, 0x00);
  ledColorBuf[3] = makeColor(0x00, 0xFF, 0xFF);
  ledColorBuf[4] = makeColor(0x00, 0x00, 0xFF);
  ledColorBuf[5] = makeColor(0xFF, 0x00, 0xFF);
  ledColorBuf[6] = makeColor(0xFF, 0xFF, 0xFF);
  ledColorBuf[7] = makeColor(0x00, 0x00, 0x00);
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

//byte fadeRed = 0;
//byte fadeGreen = 0;
//byte fadeBlue = 0;

void loop() {

  
  digitalWrite(layerManualPin, digitalRead(sw4Pin));

  if(digitalRead(sw3Pin)){
    ledColor.r = analogRead(potPin)/4;
  }
  if(digitalRead(sw2Pin)){
    ledColor.g = analogRead(potPin)/4;
  }
  if(digitalRead(sw1Pin)){
    ledColor.b = analogRead(potPin)/4;
  }

  if(colorAngle >= 360){
    colorAngle -= 360;
  }

  ledColor = rainbow[colorAngle++];

  if(digitalRead(btn1Pin) && !lastButton){
    Color last = ledColorBuf[7];
    ledColorBuf[7] = ledColorBuf[6];
    ledColorBuf[6] = ledColorBuf[5];
    ledColorBuf[5] = ledColorBuf[4];
    ledColorBuf[4] = ledColorBuf[3];
    ledColorBuf[3] = ledColorBuf[2];
    ledColorBuf[2] = ledColorBuf[1];
    ledColorBuf[1] = ledColorBuf[0];
    ledColorBuf[0] = ledColor;
  }

  //if(digitalRead(sw1Pin))fadeRed += 1;
  //if(digitalRead(sw2Pin))fadeGreen += 1;
  //if(digitalRead(sw3Pin))fadeBlue += 1;

  lastButton = digitalRead(btn1Pin);

  
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
