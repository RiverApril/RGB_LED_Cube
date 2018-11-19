//
//  ColorGame.cpp
//  
//
//  Created by Braeden Atlee on 7/20/18.
//  Copyright © 2018 Braeden Atlee. All rights reserved.
//

#include "ColorGame.hpp"

#include <stdlib.h>
#include <time.h>

ColorGame::ColorGame(Cube8* cube, signed int* inputs, signed int* lastInputs) : Game(cube, inputs, lastInputs){
    colorOptions[0] = 0xFF0000; // red
    colorOptions[1] = 0x00FF00; // green
    colorOptions[2] = 0x0000FF; // blue
    colorOptions[3] = 0xFF69B4; // pink
    colorOptions[4] = 0x808080; // gray
    colorOptions[5] = 0xFFFF00; // yellow
    
    srand((unsigned)time(0));
    
    tick = 0;
    blink = false;
    
    reset();
    
}

ColorGame::~ColorGame(){
    
}

void ColorGame::reset(){
    for(pos_t i = 0; i < 512; i++){
        gameGrid[i] = rand()%6;
        assimilated[i] = false;
    }
    
    assimilated[COORD(0, 0, 0)] = true;
    
    moves = 0;
    
    gameOver = false;
    
    spread();
}

bool ColorGame::tryToAssimilate(unsigned char c, int x, int y, int z){
    if(x >= 8 || x < 0 || y >= 8 || y < 0 || z >= 8 || z < 0){
        return false;
    }
    pos_t i = COORD(x, y, z);
    if(assimilated[i]){
        return false;
    }
    if(c == gameGrid[i]){
        assimilated[i] = true;
        return true;
    }
    return false;
}

void ColorGame::spread(){
    bool changed;
    do{
        changed = false;
        for(int i = 0; i < 512; i++){
            if(assimilated[i]){
                if(tryToAssimilate(gameGrid[i], X_COORD(i)+1, Y_COORD(i), Z_COORD(i)))changed = true;
                if(tryToAssimilate(gameGrid[i], X_COORD(i)-1, Y_COORD(i), Z_COORD(i)))changed = true;
                if(tryToAssimilate(gameGrid[i], X_COORD(i), Y_COORD(i)+1, Z_COORD(i)))changed = true;
                if(tryToAssimilate(gameGrid[i], X_COORD(i), Y_COORD(i)-1, Z_COORD(i)))changed = true;
                if(tryToAssimilate(gameGrid[i], X_COORD(i), Y_COORD(i), Z_COORD(i)+1))changed = true;
                if(tryToAssimilate(gameGrid[i], X_COORD(i), Y_COORD(i), Z_COORD(i)-1))changed = true;
            }
        }
    }while(changed);
}

bool ColorGame::checkGameOver(){
    for(pos_t i = 0; i < 512; i++){
        if(!assimilated[i]){
            return false;
        }
    }
    return true;
}

void ColorGame::changeColor(unsigned char c){
    if(gameGrid[0] == c){
        return;
    }
    for(pos_t i = 0; i < 512; i++){
        if(assimilated[i]){
            gameGrid[i] = c;
        }
    }
    spread();
    moveRecord[moves] = c;
    moves++;
    pointScroll = 0;
    if(checkGameOver()){
        gameOver = true;
    }
}

void ColorGame::update(double delta){
    
    tick += delta;
    
    cube->clearAll();
    
    if(gameOver){
        
        if(tick >= 0.25){
            tick = 0;
            pointScroll++;
        }
        
        for(pos_t i = 0; i < 512; i++){
            if(pointScroll > i && i < moves){
                cube->setVoxel(i, colorOptions[moveRecord[i]]);
            }
        }
        
        if(inputs[INPUT_START] && !lastInputs[INPUT_START]){
            reset();
        }
        
    }else{
        
        if(tick >= 0.25){
            tick = 0;
            blink = !blink;
        }
    
        for(pos_t i = 0; i < 512; i++){
            if(!assimilated[i] || blink){
                cube->setVoxel(i, colorOptions[gameGrid[i]]);
            }
        }
        
        if(inputs[INPUT_CIRCLE] && !lastInputs[INPUT_CIRCLE]){ // circle, red
            changeColor(0);
        }
        if(inputs[INPUT_CROSS] && !lastInputs[INPUT_CROSS]){ // cross, blue
            changeColor(2);
        }
        if(inputs[INPUT_TRIANGLE] && !lastInputs[INPUT_TRIANGLE]){ // triangle, green
            changeColor(1);
        }
        if(inputs[INPUT_SQUARE] && !lastInputs[INPUT_SQUARE]){ // square, pink
            changeColor(3);
        }
        if(inputs[INPUT_FORWARD] && !lastInputs[INPUT_FORWARD]){ // dpad up, yellow
            changeColor(5);
        }
        if(inputs[INPUT_BACKWARD] && !lastInputs[INPUT_BACKWARD]){ // dpad down, gray
            changeColor(4);
        }
        
    }
    
    cube->swapBuffers();
    
}

