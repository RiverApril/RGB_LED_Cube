//
//  Game.cpp
//  
//
//  Created by Braeden Atlee on 7/16/18.
//  Copyright © 2018 Braeden Atlee. All rights reserved.
//

#include "Game.hpp"

#include <stdio.h>


Game::Game(Cube8* cube, signed int* inputs, signed int* lastInputs){
    this->cube = cube;
    this->inputs = inputs;
    this->lastInputs = lastInputs;
    running = true;
}

Game::~Game(){
    
}

void Game::update(double delta){
    printf("Must override Game::update()\n");
}
