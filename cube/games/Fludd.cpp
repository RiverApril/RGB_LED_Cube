
#include "../cubeCore/cubeCore.hpp"

#include "../cubeCore/geometry.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#define pi (3.1415926535897932385)


double aimYaw = 0; // in radians (all the way around)
double aimPitch = 0; // in radians (0 to pi)

v3 aimVector(1, 0, 0);
v3 aimCenter(3.5, 3.5, 3.5);

double streamRadius = 1.5;

double waterIntensity[512];

double distanceFromRay(v3 center, v3 dir, v3 p){
    double t = fmax(0, dot(p - center, dir));
    v3 j = center + t*dir;
    return length(j - p);
}

double getColorFromIntensity(double i){
    return RGB(0, 0, (int)(i*0xFF));
}

int main(){

    JoystickCore::start();
    LightCore::start();

    bool dirty = true;

    bool running = true;

    clock_t begin = clock();

    while(running){
        JoystickCore::processEvents();

        if(JoystickCore::buttonPressed[JS_BUTTON_SELECT]){
            running = false;
            break;
        }

        clock_t end = clock();
        double delta = double(end - begin) / CLOCKS_PER_SEC;
        begin = end;

        aimYaw += JoystickCore::axis[JS_AXIS_LX]/32767.0*(2*pi*delta);
        aimPitch += JoystickCore::axis[JS_AXIS_LY]/32767.0*(2*pi*delta);
        if(aimPitch > pi){
            aimPitch = pi;
        }
        if(aimPitch < 0){
            aimPitch = 0;
        }

        aimVector.x = sin(aimPitch)*cos(aimYaw);
        aimVector.y = sin(aimPitch)*sin(aimYaw);
        aimVector.z = cos(aimPitch);

        
        LightCore::clearAll();
        for(pos_t i = 0; i < 512; i++){
            double dist = distanceFromRay(aimCenter, aimVector, V3_COORD_8(i))/streamRadius;
            waterIntensity[i] = (dist<1?1-dist:0);
            LightCore::setLight(i, getColorFromIntensity(waterIntensity[i]));
        }
        LightCore::swapBuffers();

    }

    JoystickCore::stop();
    LightCore::stop();

    return 0;
}
