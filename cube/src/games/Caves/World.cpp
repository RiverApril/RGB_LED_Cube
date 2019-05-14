#include "World.hpp"


color_t getBlockColor(Block b){
    switch(b){
        case air: return 0x000000;
        case stone: return 0xA0A0FF;
        case dirt: return 0xFF8040;
        case grass: return 0x00FF00;
        case water: return 0x0000FF;
        case wood: return 0xFFA080;
        case torch: return 0xFFFF00;
        default: printf("Huh? what is this? %d\n", b);return 0xFF00FF;
    }
}

unsigned char getLightDep(Block b){
    switch(b){
        case water: return 3;
        default: return isTransparent(b)?2:6;
    }
}

unsigned char getBlockSourceLight(Block b){
    switch(b){
        case torch: return 15;
        default: return 0;
    }
}

bool isSolid(Block b){
    switch(b){
        case air: return false;
        case torch: return false;
        default: return true;
    }
}

bool isTransparent(Block b){
    switch(b){
        case air: return true;
        case torch: return true;
        case water: return true;
        default: return false;
    }
}

void World::resetSkyLight(int x, int y){
    unsigned char brightness = 15;
    surfaceMap[y*WORLD_X_SIZE + x] = 0;

    for(int z = WORLD_Z_SIZE-1; z >= 0; z--){

        light[WORLD_INDEX(x, y, z)] = (brightness << 4) | (light[WORLD_INDEX(x, y, z)] & 0x0F);

        Block b = blocks[WORLD_INDEX(x, y, z)];
        if(!isTransparent(b)){
            if(brightness != 0){
                brightness = 0;
                surfaceMap[y*WORLD_X_SIZE + x] = z;
            }
        }
    }
}

void World::resetBlockLight(int x, int y, int z){
    resetBlockLight(x, y, z, getBlockSourceLight(getBlock(x, y, z)));
}

void World::resetBlockLight(int x, int y, int z, unsigned char source){
    light[WORLD_INDEX(x, y, z)] = (light[WORLD_INDEX(x, y, z)] & 0xF0) | (source & 0x0F);
}

void World::updateLight(int x, int y, int z){
    if(surfaceMap[y*WORLD_X_SIZE + x] == z){
        unsigned char skylight = (light[WORLD_INDEX(x, y, z)] >> 4);
        spreadSkyLight(x, y, z, skylight);
    }
    if(getBlockSourceLight(getBlock(x, y, z)) > 0){
        unsigned char blocklight = (light[WORLD_INDEX(x, y, z)] & 0x0F);
        spreadBlockLight(x, y, z, blocklight);
    }
}

void World::spreadSkyLight(int x, int y, int z, unsigned char here){
    if(here == 0) return;
    if(x < 0 || y < 0 || z < 0 || x >= WORLD_X_SIZE || y >= WORLD_Y_SIZE || z >= WORLD_Z_SIZE) return;
    unsigned char skylight = light[WORLD_INDEX(x, y, z)] >> 4;
    //if(here != 15)printf("Spread: %u ", here);
    if(skylight <= here){
        light[WORLD_INDEX(x, y, z)] = (here << 4) | (light[WORLD_INDEX(x, y, z)] & 0x0F);
        unsigned char dep = getLightDep(getBlock(x, y, z));
        if(here > dep){
            unsigned char next = here - dep;
            spreadSkyLight(x-1, y, z, next);
            spreadSkyLight(x+1, y, z, next);
            spreadSkyLight(x, y-1, z, next);
            spreadSkyLight(x, y+1, z, next);
            spreadSkyLight(x, y, z-1, next);
            spreadSkyLight(x, y, z+1, next);
        }
    }
}

void World::spreadBlockLight(int x, int y, int z, unsigned char here){
    if(here == 0) return;
    if(x < 0 || y < 0 || z < 0 || x >= WORLD_X_SIZE || y >= WORLD_Y_SIZE || z >= WORLD_Z_SIZE) return;
    unsigned char blocklight = light[WORLD_INDEX(x, y, z)] & 0x0F;
    if(blocklight <= here){
        light[WORLD_INDEX(x, y, z)] = (here) | (light[WORLD_INDEX(x, y, z)] & 0xF0);
        unsigned char dep = getLightDep(getBlock(x, y, z));
        if(here > dep){
            unsigned char next = here - dep;
            spreadBlockLight(x-1, y, z, next);
            spreadBlockLight(x+1, y, z, next);
            spreadBlockLight(x, y-1, z, next);
            spreadBlockLight(x, y+1, z, next);
            spreadBlockLight(x, y, z-1, next);
            spreadBlockLight(x, y, z+1, next);
        }
    }
}

void World::performBlockUpdate(int x, int y, int z){
    for(int i = -15; i <= 15; i++){
        int xx = x + i;
        for(int j = -15; j <= 15; j++){
            int yy = y + j;
            for(int k = -15; k <= 15; k++){
                int zz = z + k;
                if(xx < 0 || yy < 0 || zz < 0 || xx >= WORLD_X_SIZE || yy >= WORLD_Y_SIZE || zz >= WORLD_Z_SIZE){
                    continue;
                }
                resetBlockLight(xx, yy, zz);
            }
            resetSkyLight(xx, yy);
        }
    }
    for(int i = -15; i <= 15; i++){
        int xx = x + i;
        for(int j = -15; j <= 15; j++){
            int yy = y + j;
            for(int k = -15; k <= 15; k++){
                int zz = z + k;
                if(xx < 0 || yy < 0 || zz < 0 || xx >= WORLD_X_SIZE || yy >= WORLD_Y_SIZE || zz >= WORLD_Z_SIZE){
                    continue;
                }
                updateLight(xx, yy, zz);
            }
        }
    }

}