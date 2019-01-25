
#include "cubeCore.hpp"

#include <stdio.h>
#include <stdlib.h>
#include <cstring>


#ifdef __arm__

#include <wiringPi.h>
#include <cstring>


//#define PIN_Serial 14
#define PIN_Clock 30
#define PIN_Latch 21
int serialPins[8] = {14, 0, 0, 0, 0, 0, 0, 0};
int layerPins[8] = {6, 10, 11, 31, 26, 27, 28, 29};

namespace JoystickCore {
    int file;

    bool buttonDown[JS_BUTTON_COUNT];
    bool buttonDownLast[JS_BUTTON_COUNT];
    bool buttonPressed[JS_BUTTON_COUNT];
    bool buttonReleased[JS_BUTTON_COUNT];
    signed short axis[JS_AXIS_COUNT];

    bool start(){

        for(unsigned char i = 0; i < JS_BUTTON_COUNT; i++){
            buttonDown[i] = false;
            buttonDownLast[i] = false;
            buttonPressed[i] = false;
            buttonReleased[i] = false;
        }

        for(unsigned char i = 0; i < JS_AXIS_COUNT; i++){
            axis[i] = 0;
        }

        file = open("/dev/input/js0", O_RDONLY);
        return file != -1;
    }

    void stop(){
        close(file);
    }

    void processEvents(){
        Event e;

        for(unsigned char i = 0; i < JS_BUTTON_COUNT; i++){
            buttonDownLast[i] = buttonDown[i];
        }

        while (read(file, &e, sizeof(e)) > 0) {
            if ((e.type & ~JS_EVENT_INIT) == JS_EVENT_BUTTON) {
                if(e.value){
                    buttonDown[e.number] = 1;
                    //printf("Down: %d\n", e.number);
                }else{
                    buttonDown[e.number] = 0;
                    //printf("Up: %d\n", e.number);
                }
            }else if((e.type & ~JS_EVENT_INIT) == JS_EVENT_AXIS){
                axis[e.number] = e.value;
                //printf("Axis[%d]: %d\n", e.number, e.value);
            }

		}

        for(unsigned char i = 0; i < JS_BUTTON_COUNT; i++){
            buttonPressed[i] = buttonDown[i] && !buttonDownLast[i];
            buttonReleased[i] = !buttonDown[i] && buttonDownLast[i];
        }
    }
    
}


namespace LightCore {
    
    color_t buffer1[512+1]; //last bit is on while running
    color_t buffer2[512+1];

    color_t** frontBuffer;
    color_t** backBuffer;

    pthread_t thread;

    void start(){

        frontBuffer = new color_t*(&buffer1[0]);
        backBuffer = new color_t*(&buffer2[0]);

        clearAll();
        swapBuffers();
        clearAll();

        (*frontBuffer)[LT_RUNNING_BUFFER_INDEX] = 1;
        (*backBuffer)[LT_RUNNING_BUFFER_INDEX] = 1;

        pthread_create(&thread, 0, &(GPIO::threadFunc), (void*)frontBuffer);
    }

    void stop(){
        (*backBuffer)[LT_RUNNING_BUFFER_INDEX] = 0;
        swapBuffers();

        pthread_join(thread, 0);
    }
    
    namespace GPIO {

        // z-axis: layers
        // y-axis: slices
        // x-axis: shift registers
        void render(unsigned int* buffer, unsigned char layer, unsigned char bamMask){
            digitalWrite(PIN_Latch, 0);
            for(int j = 0; j < 1/*8*/; j++){ // only one slice atm
                for(int i = 0; i < 8; i++){ // red
                    digitalWrite(PIN_Clock, 0);
                    digitalWrite(serialPins[j], ((buffer[COORD_8(i, j, layer)]&0xFF0000)>>16)&bamMask);
                    digitalWrite(PIN_Clock, 1);
                }
                for(int i = 0; i < 8; i++){ // green
                    digitalWrite(PIN_Clock, 0);
                    digitalWrite(serialPins[j], ((buffer[COORD_8(i, j, layer)]&0x00FF00)>>8)&bamMask);
                    digitalWrite(PIN_Clock, 1);
                }
                for(int i = 0; i < 8; i++){ // blue
                    digitalWrite(PIN_Clock, 0);
                    digitalWrite(serialPins[j], ((buffer[COORD_8(i, j, layer)]&0x0000FF))&bamMask);
                    digitalWrite(PIN_Clock, 1);
                }
            }
            digitalWrite(PIN_Latch, 1);
        }

        void* threadFunc(void* buffer){
            
            if (wiringPiSetup() == -1){
                printf("Failed to setup wiringPi\n");
                return 0;
            }

            for(int i = 0; i < 8; i++){
                pinMode(layerPins[i], OUTPUT);
                digitalWrite(layerPins[i], 1);
            }

            for(int i = 0; i < 1/*8*/; i++){ // only one slice atm
                pinMode(serialPins[i], OUTPUT);
                digitalWrite(serialPins[i], 0);
            }

            pinMode(PIN_Clock, OUTPUT);
            digitalWrite(PIN_Clock, 0);

            pinMode(PIN_Latch, OUTPUT);
            digitalWrite(PIN_Latch, 0);

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


            for(int i = 0; i < 8; i++){
                digitalWrite(layerPins[i], 1);
            }

            return 0;
        }

    }

    void clearAll(){
        for(pos_t i = 0; i < 512; i++){
            (*backBuffer)[i] = 0;
        }
    }

    void swapBuffers(){
        color_t* temp = *frontBuffer;
        *frontBuffer = *backBuffer;
        *backBuffer = temp;
    }

    void setLight(pos_t x, pos_t y, pos_t z, color_t color){
        (*backBuffer)[COORD_8(x, y, z)] = color;;
    }

    void setLight(pos_t i, color_t color){
        (*backBuffer)[i] = color;;
    }

    void setLights(color_t buffer[512]){
        std::memcpy(*backBuffer, buffer, 512 * sizeof(color_t));
    }

}


#else

#include "visualRenderer.hpp"

namespace JoystickCore {

    bool buttonDown[JS_BUTTON_COUNT];
    bool buttonDownLast[JS_BUTTON_COUNT];
    bool buttonPressed[JS_BUTTON_COUNT];
    bool buttonReleased[JS_BUTTON_COUNT];
    signed short axis[JS_AXIS_COUNT];

    bool start(){

        for(unsigned char i = 0; i < JS_BUTTON_COUNT; i++){
            buttonDown[i] = false;
            buttonDownLast[i] = false;
            buttonPressed[i] = false;
            buttonReleased[i] = false;
        }

        for(unsigned char i = 0; i < JS_AXIS_COUNT; i++){
            axis[i] = 0;
        }

        return true;
    }

    void stop(){
        if(LightCore::vs->isRunning()){
            LightCore::vs->close();
        }
    }

    void processEvents(){

        for(unsigned char i = 0; i < JS_BUTTON_COUNT; i++){
            buttonDownLast[i] = buttonDown[i];
        }


        if(LightCore::vs->isRunning()){
            
            if(LightCore::vs->controllerCount == 0){

                buttonDown[JS_BUTTON_START] = LightCore::vs->keyboardState[VISUAL_KEY_CODE_START];
                buttonDown[JS_BUTTON_SELECT] = LightCore::vs->keyboardState[VISUAL_KEY_CODE_SELECT];
                buttonDown[JS_BUTTON_LEFT] = LightCore::vs->keyboardState[VISUAL_KEY_CODE_LEFT];
                buttonDown[JS_BUTTON_RIGHT] = LightCore::vs->keyboardState[VISUAL_KEY_CODE_RIGHT];
                buttonDown[JS_BUTTON_UP] = LightCore::vs->keyboardState[VISUAL_KEY_CODE_FORWARD];
                buttonDown[JS_BUTTON_DOWN] = LightCore::vs->keyboardState[VISUAL_KEY_CODE_BACKWARD];
                buttonDown[JS_BUTTON_A] = LightCore::vs->keyboardState[VISUAL_KEY_CODE_A];
                buttonDown[JS_BUTTON_B] = LightCore::vs->keyboardState[VISUAL_KEY_CODE_B];
                buttonDown[JS_BUTTON_X] = LightCore::vs->keyboardState[VISUAL_KEY_CODE_X];
                buttonDown[JS_BUTTON_Y] = LightCore::vs->keyboardState[VISUAL_KEY_CODE_Y];
                buttonDown[JS_BUTTON_L1] = LightCore::vs->keyboardState[VISUAL_KEY_CODE_L1];
                buttonDown[JS_BUTTON_R1] = LightCore::vs->keyboardState[VISUAL_KEY_CODE_R1];
                buttonDown[JS_BUTTON_L2] = LightCore::vs->keyboardState[VISUAL_KEY_CODE_L2];
                buttonDown[JS_BUTTON_R2] = LightCore::vs->keyboardState[VISUAL_KEY_CODE_R2];
                buttonDown[JS_BUTTON_LS] = LightCore::vs->keyboardState[VISUAL_KEY_CODE_LS];
                buttonDown[JS_BUTTON_RS] = LightCore::vs->keyboardState[VISUAL_KEY_CODE_RS];
                buttonDown[JS_BUTTON_HOME] = LightCore::vs->keyboardState[VISUAL_KEY_CODE_HOME];

            }else{
                LightCore::vs->setButtonsAndAxes(&buttonDown[0], &axis[0]);
            }

            LightCore::vs->draw();
        }else{
            LightCore::stop();
        }

        for(unsigned char i = 0; i < JS_BUTTON_COUNT; i++){
            buttonPressed[i] = buttonDown[i] && !buttonDownLast[i];
            buttonReleased[i] = !buttonDown[i] && buttonDownLast[i];
            if(buttonPressed[i]){
                printf("Button Pressed: %d\n", i);
            }
        }
    }
    
}

namespace LightCore {
    
    color_t buffer1[512+1]; //last bit is on while running
    color_t buffer2[512+1];

    color_t** frontBuffer;
    color_t** backBuffer;

    visualRenderer* vs;

    void start(){

        frontBuffer = new color_t*(&buffer1[0]);
        backBuffer = new color_t*(&buffer2[0]);

        clearAll();
        swapBuffers();
        clearAll();

        (*frontBuffer)[LT_RUNNING_BUFFER_INDEX] = 1;
        (*backBuffer)[LT_RUNNING_BUFFER_INDEX] = 1;

        vs = new visualRenderer(" visual", 640, 480);
        vs->setBuffer(frontBuffer);
    }

    void stop(){
        (*backBuffer)[LT_RUNNING_BUFFER_INDEX] = 0;
        swapBuffers();
    }

    void clearAll(){
        for(pos_t i = 0; i < 512; i++){
            (*backBuffer)[i] = 0;
        }
    }

    void swapBuffers(){
        color_t* temp = *frontBuffer;
        *frontBuffer = *backBuffer;
        *backBuffer = temp;
    }

    void setLight(pos_t x, pos_t y, pos_t z, color_t color){
        (*backBuffer)[COORD_8(x, y, z)] = color;;
    }

    void setLight(pos_t i, color_t color){
        (*backBuffer)[i] = color;;
    }

    void setLights(color_t buffer[512]){
        std::memcpy(*backBuffer, buffer, 512 * sizeof(color_t));
    }

}

#endif



