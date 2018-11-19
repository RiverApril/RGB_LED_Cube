//
//  Game.hpp
//  
//
//  Created by Braeden Atlee on 7/16/18.
//  Copyright © 2018 Braeden Atlee. All rights reserved.
//

#ifndef Game_hpp
#define Game_hpp

#define INPUT_START 0
#define INPUT_SELECT 1
#define INPUT_LEFT 2
#define INPUT_RIGHT 3
#define INPUT_FORWARD 4
#define INPUT_BACKWARD 5
#define INPUT_CIRCLE 6
#define INPUT_CROSS 7
#define INPUT_TRIANGLE 8
#define INPUT_SQUARE 9
#define INPUT_L1 10
#define INPUT_R1 11
#define INPUT_L2 12
#define INPUT_R2 13
#define INPUT_LS 14
#define INPUT_RS 15

#define pressed(k) (inputs[k] && !lastInputs[k])
#define released(k) (!inputs[k] && lastInputs[k])

class Cube8;

class Game {
    
protected:
    Cube8* cube;
    signed int* inputs;
    signed int* lastInputs;
    
    bool running;
    
public:
    Game(Cube8* cube, signed int* inputs, signed int* lastInputs);
    virtual ~Game();
    
    virtual void update(double delta);
    
    bool isRunning(){return running;};
    
};

#endif /* Game_hpp */
