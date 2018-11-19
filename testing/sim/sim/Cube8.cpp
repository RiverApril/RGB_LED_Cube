//
//  Cube8.cpp
//  
//
//  Created by Braeden Atlee on 7/15/18.
//  Copyright © 2018 Braeden Atlee. All rights reserved.
//

#include "Cube8.hpp"

#include "Cube8Renderer.hpp"

Cube8::Cube8(){
    frontBuffer = new color_t*(&buffer1[0]);
    backBuffer = new color_t*(&buffer2[0]);
    
    clearAll();
    swapBuffers();
    clearAll();
}

void Cube8::shareWithRenderer(Cube8Renderer* renderer){
    renderer->setBuffer(frontBuffer);
}

Cube8::~Cube8(){
    delete frontBuffer;
    delete backBuffer;
}

void Cube8::setBrush(color_t color){
    brush = color;
}

void Cube8::setVoxel(pos_t index){
    (*backBuffer)[index] = brush;
}

void Cube8::setVoxel(pos_t x, pos_t y, pos_t z){
    (*backBuffer)[COORD(x,y,z)] = brush;;
}

void Cube8::setVoxel(pos_t index, color_t color){
    (*backBuffer)[index] = color;
}

void Cube8::setVoxel(pos_t x, pos_t y, pos_t z, color_t color){
    (*backBuffer)[COORD(x,y,z)] = color;;
}

void Cube8::clearVoxel(pos_t index){
    (*backBuffer)[index] = 0;
}

void Cube8::clearVoxel(pos_t x, pos_t y, pos_t z){
    (*backBuffer)[COORD(x,y,z)] = 0;
}

void Cube8::clearAll(){
    for(pos_t i = 0; i < 512; i++){
        (*backBuffer)[i] = 0;
    }
}

void Cube8::swapBuffers(){
    color_t* temp = *frontBuffer;
    *frontBuffer = *backBuffer;
    *backBuffer = temp;
}




