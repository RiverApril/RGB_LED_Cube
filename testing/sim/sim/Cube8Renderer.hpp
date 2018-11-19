//
//  Cube8Renderer.hpp
//  
//
//  Created by Braeden Atlee on 7/15/18.
//  Copyright © 2018 Braeden Atlee. All rights reserved.
//

#ifndef Cube8Renderer_hpp
#define Cube8Renderer_hpp

#include "Cube8.hpp"

class Cube8Renderer {
    
    
protected:
    color_t** buffer;
    
public:
    Cube8Renderer();
    virtual ~Cube8Renderer();
    
    void setBuffer(color_t** buffer);
    virtual void draw();
    
};

#endif /* Cube8Renderer_hpp */
