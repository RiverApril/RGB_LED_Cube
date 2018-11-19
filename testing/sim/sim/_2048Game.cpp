//
//  _2048Game.cpp
//  
//
//  Created by Braeden Atlee on 7/22/18.
//  Copyright © 2018 Braeden Atlee. All rights reserved.
//

#include "_2048Game.hpp"

using namespace std;

color_t colors[64] = {
    0xFF0000,
    0xFF8000,
    0xFFFF00,
    0x80FF00,
    0x00FF80,
    0x00FFFF,
    0x0080FF,
    0x0000FF,
    0x8000FF,
    0xFF00FF,
    0xFF0080,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF,
    0xFFFFFF
};

#define COORD4(x,y,z) ((x)+(y)*4+(z)*16)
#define X_COORD4(i) ((i)%4)
#define Y_COORD4(i) (((i)/4)%4)
#define Z_COORD4(i) ((i)/16)


_2048Game::_2048Game(Cube8* cube, signed int* inputs, signed int* lastInputs) : Game(cube, inputs, lastInputs){
    
    srand((unsigned)time(0));
    
    reset();
}

_2048Game::~_2048Game(){
    
}

void _2048Game::reset(){
    for(auto t = tiles.begin(); t != tiles.end(); ++t){
        delete (*t);
    }
    tiles.clear();
    
    addNewTile();
    addNewTile();
    
}

vector<int> _2048Game::findEmpty(){
    
    int empty[64];
    
    for(int i = 0; i < 64; i++){
        empty[i] = true;
    }
    
    for(auto t = tiles.begin(); t != tiles.end(); ++t){
        empty[COORD4((*t)->xActual, (*t)->yActual, (*t)->zActual)] = false;
    }
    
    vector<int> list;
    for(int i = 0; i < 64; i++){
        if(empty[i]){
            list.push_back(i);
        }
    }
    return list;
}

void _2048Game::addNewTile(){
    Tile* t = new Tile();
    vector<int> empty = findEmpty();
    
    int pos = empty[rand()%empty.size()];
    
    t->setPos(X_COORD4(pos), Y_COORD4(pos), Z_COORD4(pos), true);
    t->num = rand()%10==0?2:1;
    t->fadeIn = 0.5;
    
    tiles.push_back(t);
    
}

Tile* _2048Game::getTileAt(int x, int y, int z){
    for(auto t = tiles.begin(); t != tiles.end(); ++t){
        if((*t)->xActual == x && (*t)->yActual == y && (*t)->zActual == z){
            return (*t);
        }
    }
    return nullptr;
}

void _2048Game::removeTile(int x, int y, int z){
    for(auto t = tiles.begin(); t != tiles.end(); ++t){
        if((*t)->xActual == x && (*t)->yActual == y && (*t)->zActual == z){
            delete (*t);
            tiles.erase(t);
            return;
        }
    }
}

bool _2048Game::shiftTile(int x, int y, int z, int dx, int dy, int dz){
    Tile* tile = getTileAt(x, y, z);
    if(tile){
        int nx = x+dx;
        int ny = y+dy;
        int nz = z+dz;
        if(nx < 0 || nx >= 4 || ny < 0 || ny >= 4 || nz < 0 || nz >= 4){
            return false;
        }
        Tile* ntile = getTileAt(nx, ny, nz);
        if(ntile){
            if(ntile->num == tile->num){
                removeTile(nx, ny, nz);
                tile->num++;
                tile->xActual = nx;
                tile->yActual = ny;
                tile->zActual = nz;
                shiftTile(nx, ny, nz, dx, dy, dz);
                return true;
            }else{
                return false;
            }
        }else{
            tile->xActual = nx;
            tile->yActual = ny;
            tile->zActual = nz;
            shiftTile(nx, ny, nz, dx, dy, dz);
            return true;
        }
    }else{
        return false;
    }
}

void _2048Game::moveSuccess(){
    addNewTile();
}

void _2048Game::moveX(bool positive){
    bool moved = false;
    for(int y = 0; y < 4; y++){
        for(int z = 0; z < 4; z++){
            if(positive){
                for(int x = 3; x >= 0; x--){
                    if(shiftTile(x, y, z, 1, 0, 0)){moved = true;}
                }
            }else{
                for(int x = 0; x < 4; x++){
                    if(shiftTile(x, y, z, -1, 0, 0)){moved = true;}
                }
            }
        }
    }
    if(moved){
        moveSuccess();
    }
}

void _2048Game::moveY(bool positive){
    bool moved = false;
    for(int x = 0; x < 4; x++){
        for(int z = 0; z < 4; z++){
            if(positive){
                for(int y = 3; y >= 0; y--){
                    if(shiftTile(x, y, z, 0, 1, 0)){moved = true;}
                }
            }else{
                for(int y = 0; y < 4; y++){
                    if(shiftTile(x, y, z, 0, -1, 0)){moved = true;}
                }
            }
        }
    }
    if(moved){
        moveSuccess();
    }
}

void _2048Game::moveZ(bool positive){
    bool moved = false;
    for(int y = 0; y < 4; y++){
        for(int x = 0; x < 4; x++){
            if(positive){
                for(int z = 3; z >= 0; z--){
                    if(shiftTile(x, y, z, 0, 0, 1)){moved = true;}
                }
            }else{
                for(int z = 0; z < 4; z++){
                    if(shiftTile(x, y, z, 0, 0, -1)){moved = true;}
                }
            }
        }
    }
    if(moved){
        moveSuccess();
    }
}

void _2048Game::update(double delta){
    
    tick += delta;
    
    cube->clearAll();
    
    for(auto t = tiles.begin(); t != tiles.end(); ++t){
        
        if((*t)->fadeIn > 0){
            (*t)->fadeIn -= delta;
        }else{
            cube->setVoxel((int)((*t)->xDraw*2), (int)((*t)->yDraw*2), (int)((*t)->zDraw*2), colors[(*t)->num-1]);
            cube->setVoxel((int)((*t)->xDraw*2), (int)((*t)->yDraw*2), (int)((*t)->zDraw*2+1), colors[(*t)->num-1]);
            cube->setVoxel((int)((*t)->xDraw*2), (int)((*t)->yDraw*2+1), (int)((*t)->zDraw*2), colors[(*t)->num-1]);
            cube->setVoxel((int)((*t)->xDraw*2), (int)((*t)->yDraw*2+1), (int)((*t)->zDraw*2+1), colors[(*t)->num-1]);
            cube->setVoxel((int)((*t)->xDraw*2+1), (int)((*t)->yDraw*2), (int)((*t)->zDraw*2), colors[(*t)->num-1]);
            cube->setVoxel((int)((*t)->xDraw*2+1), (int)((*t)->yDraw*2), (int)((*t)->zDraw*2+1), colors[(*t)->num-1]);
            cube->setVoxel((int)((*t)->xDraw*2+1), (int)((*t)->yDraw*2+1), (int)((*t)->zDraw*2), colors[(*t)->num-1]);
            cube->setVoxel((int)((*t)->xDraw*2+1), (int)((*t)->yDraw*2+1), (int)((*t)->zDraw*2+1), colors[(*t)->num-1]);
        }
        
        if(tick >= 0.1){
            if((*t)->xDraw > (*t)->xActual){ (*t)->xDraw -= 0.5; }
            if((*t)->xDraw < (*t)->xActual){ (*t)->xDraw += 0.5; }
            if((*t)->yDraw > (*t)->yActual){ (*t)->yDraw -= 0.5; }
            if((*t)->yDraw < (*t)->yActual){ (*t)->yDraw += 0.5; }
            if((*t)->zDraw > (*t)->zActual){ (*t)->zDraw -= 0.5; }
            if((*t)->zDraw < (*t)->zActual){ (*t)->zDraw += 0.5; }
        }
    }
    
    if(tick >= 0.1){
        tick = 0;
    }
    
    if(pressed(INPUT_LEFT)){ moveX(false); }
    if(pressed(INPUT_RIGHT)){ moveX(true); }
    if(pressed(INPUT_BACKWARD)){ moveZ(false); }
    if(pressed(INPUT_FORWARD)){ moveZ(true); }
    if(pressed(INPUT_L1)){ moveY(false); }
    if(pressed(INPUT_R1)){ moveY(true); }
    
    cube->swapBuffers();
    
}

