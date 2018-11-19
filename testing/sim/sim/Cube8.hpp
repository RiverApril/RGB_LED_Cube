//
//  Cube8.hpp
//  
//
//  Created by Braeden Atlee on 7/15/18.
//  Copyright © 2018 Braeden Atlee. All rights reserved.
//

#ifndef Cube_hpp
#define Cube_hpp

#define COORD(x,y,z) ((x)+(y)*8+(z)*8*8)
#define X_COORD(i) ((i)%8)
#define Y_COORD(i) (((i)/8)%8)
#define Z_COORD(i) ((i)/64)

typedef unsigned int color_t;
typedef unsigned short pos_t;

class Cube8Renderer;

class Cube8 {
    
    color_t buffer1[512];
    color_t buffer2[512];
    
    color_t** frontBuffer;
    color_t** backBuffer;
    
    color_t brush;
    
public:
    Cube8();
    ~Cube8();
    
    void shareWithRenderer(Cube8Renderer* renderer);
    
    void setBrush(color_t color);
    
    void setVoxel(pos_t index);
    void setVoxel(pos_t x, pos_t y, pos_t z);
    void setVoxel(pos_t index, color_t color);
    void setVoxel(pos_t x, pos_t y, pos_t z, color_t color);
    
    void clearVoxel(pos_t index);
    void clearVoxel(pos_t x, pos_t y, pos_t z);
    
    void clearAll();
    
    void swapBuffers();
    
    
};

#endif /* Cube_hpp */
