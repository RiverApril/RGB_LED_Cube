//
//  Cube8VisualRenderer.cpp
//  
//
//  Created by Braeden Atlee on 7/16/18.
//  Copyright © 2018 Braeden Atlee. All rights reserved.
//

#include "Cube8VisualRenderer.hpp"

#include <SDL2/SDL.h>
#include <vector>

using namespace std;

extern const unsigned long VISUAL_KEY_CODE_START = SDL_SCANCODE_RETURN;
extern const unsigned long VISUAL_KEY_CODE_SELECT = SDL_SCANCODE_RSHIFT;
extern const unsigned long VISUAL_KEY_CODE_LEFT = SDL_SCANCODE_A;
extern const unsigned long VISUAL_KEY_CODE_RIGHT = SDL_SCANCODE_D;
extern const unsigned long VISUAL_KEY_CODE_FORWARD = SDL_SCANCODE_W;
extern const unsigned long VISUAL_KEY_CODE_BACKWARD = SDL_SCANCODE_S;
extern const unsigned long VISUAL_KEY_CODE_A = SDL_SCANCODE_L;
extern const unsigned long VISUAL_KEY_CODE_B = SDL_SCANCODE_K;
extern const unsigned long VISUAL_KEY_CODE_X = SDL_SCANCODE_I;
extern const unsigned long VISUAL_KEY_CODE_Y = SDL_SCANCODE_J;
extern const unsigned long VISUAL_KEY_CODE_L1 = SDL_SCANCODE_Q;
extern const unsigned long VISUAL_KEY_CODE_R1 = SDL_SCANCODE_E;
extern const unsigned long VISUAL_KEY_CODE_L2 = SDL_SCANCODE_U;
extern const unsigned long VISUAL_KEY_CODE_R2 = SDL_SCANCODE_O;
extern const unsigned long VISUAL_KEY_CODE_LS = SDL_SCANCODE_X;
extern const unsigned long VISUAL_KEY_CODE_RS = SDL_SCANCODE_COMMA;

Cube8VisualRenderer::Cube8VisualRenderer(string title, int windowWidth, int windowHeight){
    
    this->windowWidth = windowWidth;
    this->windowHeight = windowHeight;
    
    running = false;
    
    if(SDL_Init(SDL_INIT_VIDEO) < 0){
        printf("Failed to initalize SDL. Error: %s\n", SDL_GetError());
        return;
    }
    
    mainWindow = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, windowWidth, windowHeight, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    if(!mainWindow){
        printf("Failed to create window. Error: %s\n", SDL_GetError());
        return;
    }
    
    renderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED);
    if(!renderer){
        printf("SDL_CreateRenderer: %s\n", SDL_GetError());
        return;
    }
    
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    
    keyboardState = SDL_GetKeyboardState(NULL);
    
    beginTick = SDL_GetTicks();
    
    running = true;
    
    cameraPos.x = 0;
    cameraPos.y = 0;
    cameraPos.z = 0;
    
    cameraAngle.x = 0;
    cameraAngle.y = 0;
    cameraAngle.z = 0;
    
    double fov = M_PI / 4;
    
    cameraE.x = 0;
    cameraE.y = 0;
    cameraE.z = 1/tan(fov/2);
    
    
    jigYaw = M_PI / 4;
    jigPitch = - M_PI / 4;
    jigDist = 10;
    
}

void Cube8VisualRenderer::project(double ax, double ay, double az, double& bx, double& by, double& size){
    
    double px = ax - cameraPos.x;
    double py = ay - cameraPos.y;
    double pz = az - cameraPos.z;
    
    double sx = sin(cameraAngle.x);
    double sy = sin(cameraAngle.y);
    double sz = sin(cameraAngle.z);
    double cx = cos(cameraAngle.x);
    double cy = cos(cameraAngle.y);
    double cz = cos(cameraAngle.z);
    
    double dx = cy * (sz * py + cz * px) - sy * pz;
    double dy = sx * (cy * pz + sy * (sz * py + cz * px)) + cx * (cz * py - sz * px);
    double dz = cx * (cy * pz + sy * (sz * py + cz * px)) - sx * (cz * py - sz * px);
    
    //printf("dz: %f\n", dz);
    
    bx = (cameraE.z/dz) * dx + cameraE.x;
    by = (cameraE.z/dz) * dy + cameraE.y;
    
    size = 1/sqrt(px*px + py*py + pz*pz);
}


void Cube8VisualRenderer::draw(){
    SDL_RenderClear(renderer);
    
    SDL_SetRenderDrawColor(renderer, 0xCC, 0xEE, 0xFF, 255); 
    SDL_RenderFillRect(renderer, NULL);
    
    SDL_Rect r;
    
    double bx, by;
    double size;
    color_t color;
    
    double w2 = windowWidth/2;
    double h2 = windowHeight/2;
    
    pixel pixels[512];
    int i = 0;
    
    vector<int> pixelZList;
    
    for(pos_t x = 0; x < 8; x++){
        for(pos_t y = 0; y < 8; y++){
            for(pos_t z = 0; z < 8; z++){
                project(x-3.5, y-3.5, z-3.5, bx, by, size);
                bx *= drawScale;
                by *= drawScale;
                size *= dotScale;
                
                pixels[i].x = (bx - size) + w2;
                pixels[i].y = (by - size) + h2;
                pixels[i].color = (*buffer)[COORD(x,7-y,z)];
                pixels[i].size = size;
                
                auto it = pixelZList.begin();
                while(it != pixelZList.end() && pixels[*it].size < pixels[i].size){
                    ++it;
                }
                pixelZList.insert(it, i);
                
                i++;
                
            }
        }
    }
    
    unsigned char red, green, blue, alpha;
    int k = 0;
    
    for(int j = 0; j < i; j++){
        k = pixelZList[j];
        r.x = pixels[k].x;
        r.y = pixels[k].y;
        r.w = pixels[k].size*2;
        r.h = pixels[k].size*2;
        color = pixels[k].color;
        
        red = (color&0xFF0000)>>16;
        green = (color&0xFF00)>>8;
        blue = color&0xFF;
        
        alpha = red>green?red:green;
        alpha = alpha>blue?alpha:blue;
        
        SDL_SetRenderDrawColor(renderer, red, green, blue, alpha);
        SDL_RenderFillRect(renderer, &r);
        SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0x44);
        SDL_RenderDrawRect(renderer, &r);
    }
    
    //printf("r: %d, %d\n", r.x, r.y);
    //printf("b: %f, %f\n", bx, by);
    
    anim += delta;
    
    //jigYaw = anim/10.0 * 2*M_PI;
    
    const double rotSpeed = M_PI; // per second
    
    if(keyboardState[SDL_SCANCODE_LEFT]){
        jigYaw += rotSpeed*delta;
    }
    if(keyboardState[SDL_SCANCODE_RIGHT]){
        jigYaw -= rotSpeed*delta;
    }
    if(keyboardState[SDL_SCANCODE_UP]){
        jigPitch -= rotSpeed*delta;
    }
    if(keyboardState[SDL_SCANCODE_DOWN]){
        jigPitch += rotSpeed*delta;
    }
    
    if(jigPitch > M_PI / 2){
        jigPitch = M_PI / 2;
    }
    if(jigPitch < - M_PI / 2){
        jigPitch = - M_PI / 2;
    }
    
    if(keyboardState[SDL_SCANCODE_MINUS]){
        jigDist += rotSpeed*delta;
    }
    if(keyboardState[SDL_SCANCODE_EQUALS]){
        jigDist -= rotSpeed*delta;
    }
    
    if(keyboardState[SDL_SCANCODE_LEFTBRACKET]){
        dotScale -= 50*delta;
    }
    if(keyboardState[SDL_SCANCODE_RIGHTBRACKET]){
        dotScale += 50*delta;
    }
    
    cameraAngle.y = jigYaw;;
    cameraAngle.x = jigPitch;
    
    cameraPos.x = jigDist*cos(-jigYaw - M_PI / 2)*-sin(-jigPitch - M_PI / 2);
    cameraPos.z = jigDist*sin(-jigYaw - M_PI / 2)*-sin(-jigPitch - M_PI / 2);
    
    cameraPos.y = -jigDist*cos(-jigPitch - M_PI / 2);
    

    SDL_RenderPresent(renderer);
    
    SDL_Event e;
    while(SDL_PollEvent(&e)){
        switch(e.type){
            case SDL_QUIT:{
                running = false;
                return;
            }
        }
    };
    
    endTick = SDL_GetTicks();
    delta = double(endTick - beginTick) / 1000.0;
    beginTick = SDL_GetTicks();
}
