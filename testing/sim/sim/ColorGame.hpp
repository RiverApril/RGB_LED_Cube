//
//  ColorGame.hpp
//  
//
//  Created by Braeden Atlee on 7/20/18.
//  Copyright © 2018 Braeden Atlee. All rights reserved.
//

#ifndef ColorGame_hpp
#define ColorGame_hpp

#include "Game.hpp"

#include "Cube8.hpp"

class ColorGame : public Game {
    
    unsigned char gameGrid[512];
    bool assimilated[512];
    unsigned char moveRecord[512];
    
    double tick;
    bool blink;
    
    const static int colorCount = 6;
    color_t colorOptions[colorCount];
    
    int moves;
    int pointScroll;
    
    bool gameOver;
    
public:
    ColorGame(Cube8* cube, signed int* inputs, signed int* lastInputs);
    ~ColorGame();
    
    void spread();
    bool tryToAssimilate(unsigned char c, int x, int y, int z);
    void changeColor(unsigned char c);
    void reset();
    bool checkGameOver();
    
    void update(double delta);
    
};

#endif /* ColorGame_hpp */
