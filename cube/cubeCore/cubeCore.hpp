
#ifndef cubeCore_hpp
#define cubeCore_hpp

#include <unistd.h>
#include <fcntl.h>

#ifdef __arm__
#include <pthread.h>
#endif

#define JS_EVENT_BUTTON 0x01
#define JS_EVENT_AXIS 0x02
#define JS_EVENT_INIT 0x80


#define JS_BUTTON_COUNT 17

#define JS_BUTTON_START 0
#define JS_BUTTON_SELECT 1
#define JS_BUTTON_LEFT 2
#define JS_BUTTON_RIGHT 3
#define JS_BUTTON_UP 4
#define JS_BUTTON_DOWN 5
#define JS_BUTTON_X 6
#define JS_BUTTON_Y 7
#define JS_BUTTON_B 8
#define JS_BUTTON_A 9
#define JS_BUTTON_L1 10
#define JS_BUTTON_L2 11
#define JS_BUTTON_R1 12
#define JS_BUTTON_R2 13
#define JS_BUTTON_LS 14
#define JS_BUTTON_RS 15
#define JS_BUTTON_HOME 16


#define JS_AXIS_COUNT 4

#define JS_AXIS_LX 0
#define JS_AXIS_LY 1
#define JS_AXIS_RX 2
#define JS_AXIS_RY 3


typedef unsigned int color_t;
typedef unsigned short pos_t;

#ifndef __arm__
class visualRenderer;
#endif

namespace JoystickCore {

    #ifdef __arm__
    extern int file;

    struct Event {
        unsigned int time;
        signed short value;
        unsigned char type;
        unsigned char number;
    };

    #endif

    extern bool buttonDown[JS_BUTTON_COUNT];
    extern bool buttonDownLast[JS_BUTTON_COUNT];
    extern bool buttonPressed[JS_BUTTON_COUNT];
    extern bool buttonReleased[JS_BUTTON_COUNT];
    extern signed short axis[JS_AXIS_COUNT];

    bool start();
    void stop();

    void processEvents();
    
}

#define COORD_N(x,y,z,n) ((x)+(y)*(n)+(z)*(n)*(n))
#define X_COORD_N(i,n) ((i)%(n))
#define Y_COORD_N(i,n) (((i)/(n))%(n))
#define Z_COORD_N(i,n) ((i)/((n)*(n)))

#define COORD_8(x,y,z) COORD_N(x,y,z,8)
#define X_COORD_8(i) X_COORD_N(i,8)
#define Y_COORD_8(i) Y_COORD_N(i,8)
#define Z_COORD_8(i) Z_COORD_N(i,8)

#define COORD_4(x,y,z) COORD_N(x,y,z,4)
#define X_COORD_4(i) X_COORD_N(i,4)
#define Y_COORD_4(i) Y_COORD_N(i,4)
#define Z_COORD_4(i) Z_COORD_N(i,4)

#define RGB(r,g,b) (((r)<<16)|((g)<<8)|(b))

#define PI (3.14159265358979323)

#define LT_RUNNING_BUFFER_INDEX 512

namespace LightCore {

    extern color_t buffer1[512+1];
    extern color_t buffer2[512+1];

    extern color_t** frontBuffer;
    extern color_t** backBuffer;

    #ifdef __arm__
    extern pthread_t thread;

    namespace GPIO {

        void render(unsigned int* buffer, unsigned char layer, unsigned char bamMask);

        void* threadFunc(void* buffer);

    }
    #else
    extern visualRenderer* vs;
    #endif

    void start();
    void stop();

    void clearAll();
    void swapBuffers();
    void setLight(pos_t x, pos_t y, pos_t z, color_t color);
    void setLight(pos_t i, color_t color);
    void setLights(color_t buffer[512]);

}

#endif