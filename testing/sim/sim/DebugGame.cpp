//
//  DebugGame.cpp
//  
//
//  Created by Braeden Atlee on 7/16/18.
//  Copyright © 2018 Braeden Atlee. All rights reserved.
//

#include "DebugGame.hpp"

#include "Cube8.hpp"

#include <stdlib.h>
#include <time.h>

DebugGame::DebugGame(Cube8* cube, signed int* inputs, signed int* lastInputs) : Game(cube, inputs, lastInputs){
    srand((unsigned)time(0));
    for(pos_t i = 0; i < 512; i++){
        cube->setVoxel(i, rand()&0xFFFFFF);
    }
    cube->swapBuffers();
}

DebugGame::~DebugGame(){
    
}

void DebugGame::update(double delta){
    
}


