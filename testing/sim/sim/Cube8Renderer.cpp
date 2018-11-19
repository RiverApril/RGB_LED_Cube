//
//  Cube8Renderer.cpp
//  
//
//  Created by Braeden Atlee on 7/15/18.
//  Copyright © 2018 Braeden Atlee. All rights reserved.
//

#include "Cube8Renderer.hpp"

#include <stdio.h>


Cube8Renderer::Cube8Renderer(){
    
}

Cube8Renderer::~Cube8Renderer(){
    
}

void Cube8Renderer::setBuffer(color_t** buffer){
    this->buffer = buffer;
}

void Cube8Renderer::draw(){
    printf("Must override Cube8Renderer::draw()\n");
}
