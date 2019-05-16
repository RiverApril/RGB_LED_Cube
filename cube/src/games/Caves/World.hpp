#pragma once

#include "../../core/cubeCore.hpp"
#include "../../core/geometry.hpp"

#include <vector>
#include <cmath>

using namespace std;


#define WORLD_X_SIZE 128
#define WORLD_Y_SIZE 128
#define WORLD_Z_SIZE 64

#define WORLD_SIZE (WORLD_X_SIZE*WORLD_Y_SIZE*WORLD_Z_SIZE)

#define WORLD_INDEX(x, y, z) ((z)*WORLD_X_SIZE*WORLD_Y_SIZE + (y)*WORLD_X_SIZE + (x))

enum Block: unsigned char{
    air, stone, dirt, grass, water, wood, torch, BLOCK_COUNT
};

color_t getBlockColor(Block b);
unsigned char getLightDep(Block b);
unsigned char getBlockSourceLight(Block b);
bool isSolid(Block b);
bool isTransparent(Block b);

class Entity;

class World{
public:

    Block blocks[WORLD_SIZE];
    unsigned char light[WORLD_SIZE];
    int surfaceMap[WORLD_X_SIZE * WORLD_Y_SIZE];

    vector<Entity*> entities;


    Block getBlock(int x, int y, int z){
        if(x < 0 || y < 0 || z < 0 || x >= WORLD_X_SIZE || y >= WORLD_Y_SIZE || z >= WORLD_Z_SIZE){
            return stone;
        }
        return blocks[WORLD_INDEX(x, y, z)];
    }

    Block getBlock(p3 pos){
        return getBlock(pos.x, pos.y, pos.z);
    }

    unsigned char timeShift(unsigned char light, double timeOfDay){
        double brightness = min(sin(timeOfDay*2*3.1415926535897932)+1, 1.0);
        return (unsigned char)(light * brightness);
    }

    unsigned char getLight(int x, int y, int z, double timeOfDay){
        if(x < 0 || y < 0 || z < 0 || x >= WORLD_X_SIZE || y >= WORLD_Y_SIZE || z >= WORLD_Z_SIZE){
            return 0;
        }
        return max(timeShift(light[WORLD_INDEX(x, y, z)] >> 4, timeOfDay) & 0xF, light[WORLD_INDEX(x, y, z)] & 0xF);
    }

    void setBlock(p3 pos, Block b, bool update = true){
        setBlock(pos.x, pos.y, pos.z, b, update);
    }

    void setBlock(int x, int y, int z, Block b, bool update = true){
        if(x < 0 || y < 0 || z < 0 || x >= WORLD_X_SIZE || y >= WORLD_Y_SIZE || z >= WORLD_Z_SIZE){
            return;
        }
        blocks[WORLD_INDEX(x, y, z)] = b;
        if(update){
            performBlockUpdate(x, y, z);
        }
    }

    void performBlockUpdate(int x, int y, int z);

    void resetSkyLight(int x, int y);

    void resetBlockLight(int x, int y, int z);
    void resetBlockLight(int x, int y, int z, unsigned char source);
    void updateLight(int x, int y, int z);

    void spreadSkyLight(int x, int y, int z, unsigned char from);
    void spreadBlockLight(int x, int y, int z, unsigned char from);

};
