
#include "../core/cubeCore.hpp"
#include "../core/geometry.hpp"
#include "../core/perlin.hpp"

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <algorithm>

using namespace std;

#define pi (3.1415926535897932)

#define CUBE_ANGLE (0.61547970867038734) // asin(1/sqrt(3))

#define WATER_POWER 10.0
#define SLIME_SPREAD 0.0001

v3 aimVector(1, 0, 0);
v3 aimCenter(0, 0, 7);

double streamRadius = 1.5;

double slime[3*8*8];

double timer = 0.0;

double getSlimeAt(int index){
    if(index < 0 || index >= 3*8*8){
        return 0.0;
    }
    return slime[index];
}

double brightnessFix(double x){
    x-=1;
    return 1-x*x*x*x;
}

double distanceFromRay(v3 center, v3 dir, v3 p){
    double t = max(0.0, dot(p - center, dir));
    v3 j = center + t*dir;
    return length(j - p);
}

int getSlimeIndex(int i){
    if(X_COORD_8(i) == 7){
        return 0*8*8 + Y_COORD_8(i) * 8 + Z_COORD_8(i);
    }else if(Y_COORD_8(i) == 7){
        return 1*8*8 + X_COORD_8(i) * 8 + Z_COORD_8(i);
    }else if(Z_COORD_8(i) == 0){
        return 2*8*8 + X_COORD_8(i) * 8 + Y_COORD_8(i);
    }else{
        return -1;
    }
}

double getColorFromIntensity(double w, double s, int x, int y, int z){
    w = brightnessFix(w);
    s = brightnessFix(s);
    double extra = 0.0;
    if(w+s > 1.0){
        extra = w+s-1.0;
    }
    return RGB(
        (int)(min(s+extra, 1.0)*0xFF), 
        (int)(min(w/2.0+extra, 1.0)*0xFF), 
        (int)(min(w+s, 1.0)*0xFF));
}

int main(){

    JoystickCore::start();
    LightCore::start();

    bool dirty = true;

    bool running = true;

    clock_t begin = clock();

    for(int i = 0; i < 3*8*8; i++){
        slime[i] = rand()*1.0/RAND_MAX;
    }

    while(running){
        JoystickCore::processEvents();

        if(JoystickCore::buttonPressed[JS_BUTTON_HOME]){
            running = false;
            break;
        }

        clock_t end = clock();
        double delta = double(end - begin) / CLOCKS_PER_SEC;
        begin = end;

        timer += delta;

        /*aimYaw += JoystickCore::axis[JS_AXIS_LX]/AXIS_MAX*(2*pi*delta);
        aimPitch += JoystickCore::axis[JS_AXIS_LY]/AXIS_MAX*(2*pi*delta);
        if(aimPitch > pi){
            aimPitch = pi;
        }
        if(aimPitch < 0){
            aimPitch = 0;
        }

        aimVector.x = sin(aimPitch)*cos(aimYaw);
        aimVector.y = sin(aimPitch)*sin(aimYaw);
        aimVector.z = cos(aimPitch);*/

        /*aimVector.x = (JoystickCore::axis[JS_AXIS_LX]/AXIS_MAX);
        aimVector.y = (JoystickCore::axis[JS_AXIS_LY]/AXIS_MAX);
        aimVector.z = (JoystickCore::axis[JS_AXIS_RY]/AXIS_MAX);*/

        double ytilt = AXIS_DOUBLE(JoystickCore::axis[JS_AXIS_LY]);
        double phi = ytilt * ( (ytilt>0)?(pi/2-CUBE_ANGLE):(CUBE_ANGLE) ) + pi/2;
        double theta = AXIS_DOUBLE(JoystickCore::axis[JS_AXIS_LX]) * (pi/4);

        aimVector.x = sin(phi)*cos(theta);
        aimVector.y = sin(phi)*sin(theta);
        aimVector.z = cos(phi);

        // Rotate aorund X
        // const double t2 = pi/4.0;
        // double ynew = aimVector.y * cos(t2) + aimVector.z * -sin(t2);
        // aimVector.z = aimVector.y * sin(t2) + aimVector.z * cos(t2);
        // aimVector.y = ynew;

        // Rotate aorund Y
        const double t2 = CUBE_ANGLE;
        double tmp2 = aimVector.x * cos(t2) + aimVector.z * sin(t2);
        aimVector.z = aimVector.x * -sin(t2) + aimVector.z * cos(t2);
        aimVector.x = tmp2;

        // Rotate around Z
        const double t1 = pi/4;
        double tmp1 = aimVector.x * cos(t1) + aimVector.y * -sin(t1);
        aimVector.y = aimVector.x * sin(t1) + aimVector.y * cos(t1);
        aimVector.x = tmp1;

        aimVector.x = max(aimVector.x, 0.0);
        aimVector.y = max(aimVector.y, 0.0);
        aimVector.z = min(aimVector.z, 0.0);

        aimVector = normalize(aimVector);
        
        LightCore::clearAll();
        for(pos_t i = 0; i < 512; i++){
            double dist = distanceFromRay(aimCenter, aimVector, V3_COORD_8(i))/streamRadius;
            double waterIntensity = (dist<1?1-dist:0);
            int slimeIndex = getSlimeIndex(i);
            double slimeIntensity = 0;
            int x = X_COORD_8(i);
            int y = Y_COORD_8(i);
            int z = Z_COORD_8(i);
            if(slimeIndex != -1){
                double growth = 0.0;
                if(x < 7) growth += getSlimeAt(getSlimeIndex(COORD_8(x+1, y, z)));
                if(x > 0) growth += getSlimeAt(getSlimeIndex(COORD_8(x-1, y, z)));
                if(y < 7) growth += getSlimeAt(getSlimeIndex(COORD_8(x, y+1, z)));
                if(y > 0) growth += getSlimeAt(getSlimeIndex(COORD_8(x, y-1, z)));
                if(z < 7) growth += getSlimeAt(getSlimeIndex(COORD_8(x, y, z+1)));
                if(z > 0) growth += getSlimeAt(getSlimeIndex(COORD_8(x, y, z-1)));
                growth *= SLIME_SPREAD;
                slime[slimeIndex] += growth;
                if(slime[slimeIndex] > 1.0){
                    slime[slimeIndex] = 1.0;
                }

                double power = waterIntensity * WATER_POWER * delta;
                if(slime[slimeIndex] > power){
                    slime[slimeIndex] -= power;
                }else{
                    slime[slimeIndex] = 0.0;
                }
                slimeIntensity = slime[slimeIndex];
            }
            LightCore::setLight(i, getColorFromIntensity(waterIntensity, slimeIntensity, x, y, z));
        }
        LightCore::swapBuffers();

    }

    JoystickCore::stop();
    LightCore::stop();

    return 0;
}
