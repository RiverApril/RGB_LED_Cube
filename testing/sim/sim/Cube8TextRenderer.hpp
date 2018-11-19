//
//  Cube8TextRenderer.hpp
//  
//
//  Created by Braeden Atlee on 7/15/18.
//  Copyright © 2018 Braeden Atlee. All rights reserved.
//

#ifndef Cube8TextRenderer_hpp
#define Cube8TextRenderer_hpp

#include "Cube8Renderer.hpp"


class Cube8TextRenderer : public Cube8Renderer {

public:
    Cube8TextRenderer();
    virtual ~Cube8TextRenderer();
    
    virtual void draw();
    
};

#endif /* Cube8TextRenderer_hpp */
