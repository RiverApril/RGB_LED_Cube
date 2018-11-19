//
//  Cube8VisualRenderer.hpp
//  
//
//  Created by Braeden Atlee on 7/16/18.
//  Copyright © 2018 Braeden Atlee. All rights reserved.
//

#ifndef Cube8VisualRenderer_hpp
#define Cube8VisualRenderer_hpp

extern const unsigned long VISUAL_KEY_CODE_START;
extern const unsigned long VISUAL_KEY_CODE_SELECT;
extern const unsigned long VISUAL_KEY_CODE_LEFT;
extern const unsigned long VISUAL_KEY_CODE_RIGHT;
extern const unsigned long VISUAL_KEY_CODE_FORWARD;
extern const unsigned long VISUAL_KEY_CODE_BACKWARD;
extern const unsigned long VISUAL_KEY_CODE_A;
extern const unsigned long VISUAL_KEY_CODE_B;
extern const unsigned long VISUAL_KEY_CODE_X;
extern const unsigned long VISUAL_KEY_CODE_Y;
extern const unsigned long VISUAL_KEY_CODE_L1;
extern const unsigned long VISUAL_KEY_CODE_R1;
extern const unsigned long VISUAL_KEY_CODE_L2;
extern const unsigned long VISUAL_KEY_CODE_R2;
extern const unsigned long VISUAL_KEY_CODE_LS;
extern const unsigned long VISUAL_KEY_CODE_RS;

#include <string>

#include "Cube8Renderer.hpp"

class SDL_Window;
class SDL_Renderer;

struct v3 {
    double x, y, z;
};

struct pixel {
    double x, y, size;
    color_t color;
};

class Cube8VisualRenderer : public Cube8Renderer {
    
    SDL_Window* mainWindow;
    SDL_Renderer* renderer;
    
    const unsigned char* keyboardState;
    
    int windowWidth, windowHeight;
    
    bool running;
    
    v3 cameraPos;
    v3 cameraAngle;
    v3 cameraE;
    
    void project(double ax, double ay, double az, double& bx, double& by, double& size);
    
    double anim = 0;
    double jigYaw;
    double jigPitch;
    double jigDist;
    double drawScale = 100;
    double dotScale = 30;
    
    unsigned int beginTick, endTick;
    double delta;
    
public:
    Cube8VisualRenderer(std::string title, int windowWidth, int windowHeight);
    
    virtual void draw();
    
    bool isRunning(){return running;}
    const unsigned char* getKeyboardState(){return keyboardState;}
    double getDelta(){return delta;}
    
    
};

#endif /* Cube8VisualRenderer_hpp */
