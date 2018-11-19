//
//  DebugGame.hpp
//  
//
//  Created by Braeden Atlee on 7/16/18.
//  Copyright © 2018 Braeden Atlee. All rights reserved.
//

#ifndef DebugGame_hpp
#define DebugGame_hpp

#include "Game.hpp"

class DebugGame : public Game {
    
public:
    DebugGame(Cube8* cube, signed int* inputs, signed int* lastInputs);
    ~DebugGame();
    
    void update(double delta);
    
};

#endif /* DebugGame_hpp */
