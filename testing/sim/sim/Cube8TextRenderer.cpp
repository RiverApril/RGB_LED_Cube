//
//  Cube8TextRenderer.cpp
//  
//
//  Created by Braeden Atlee on 7/15/18.
//  Copyright © 2018 Braeden Atlee. All rights reserved.
//

#include "Cube8TextRenderer.hpp"

#include <stdio.h>

Cube8TextRenderer::Cube8TextRenderer() : Cube8Renderer(){
    
}

Cube8TextRenderer::~Cube8TextRenderer(){
    
}

color_t to3dig(color_t color){
    return (((color&0xFF0000)>>12)&0xF00) | (((color&0x00FF00)>>8)&0x0F0) | (((color&0x0000FF)>>4)&0x00F);
}

void Cube8TextRenderer::draw(){
    
    for(pos_t i = 0; i < 8; i+=4){
        for(pos_t y = 0; y < 8; y++){
            for(pos_t z = i; z < i+4; z++){
                for(pos_t x = 0; x < 8; x++){
                    color_t d = (*buffer)[COORD(x,y,z)];
                    if(d){
                        printf("%03x", to3dig(d));
                   }else{
                       printf(" - ");
                   }
                }
                printf("  ");
            }
            printf("\n");
        }
        printf("\n");
    }
    printf("\n");
    
}
