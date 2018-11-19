//
//  _2048Game.hpp
//  
//
//  Created by Braeden Atlee on 7/22/18.
//  Copyright © 2018 Braeden Atlee. All rights reserved.
//

#ifndef _2048Game_hpp
#define _2048Game_hpp

#include "Game.hpp"

#include "Cube8.hpp"

#include <vector>


extern color_t colors[64];

struct Tile {
    double xDraw, yDraw, zDraw;
    int xActual, yActual, zActual;
    int num;
    double fadeIn;
    
    void setPos(int x, int y, int z, bool setDraw = false){
        xActual = x;
        yActual = y;
        zActual = z;
        if(setDraw){
            xDraw = x;
            yDraw = y;
            zDraw = z;
        }
    }
};

class _2048Game : public Game {
    
    std::vector<Tile*> tiles;
    
    double tick;
    
public:
    _2048Game(Cube8* cube, signed int* inputs, signed int* lastInputs);
    ~_2048Game();
    
    void reset();
    std::vector<int> findEmpty();
    void addNewTile();
    
    void removeTile(int x, int y, int z);
    Tile* getTileAt(int x, int y, int z);
    bool shiftTile(int x, int y, int z, int dx, int dy, int dz);
    void moveSuccess();
    void moveX(bool positive);
    void moveY(bool positive);
    void moveZ(bool positive);
    
    
    void update(double delta);
    
};

#endif /* _2048Game_hpp */
