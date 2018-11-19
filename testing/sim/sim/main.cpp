//
//  main.cpp
//  
//
//  Created by Braeden Atlee on 7/15/18.
//  Copyright © 2018 Braeden Atlee. All rights reserved.
//

#include <iostream>

#include "Cube8.hpp"
#include "Cube8TextRenderer.hpp"
#include "Cube8VisualRenderer.hpp"
#include "DebugGame.hpp"
#include "ColorGame.hpp"
#include "_2048Game.hpp"

using namespace std;

int main(int argc, const char * argv[]) {
    cout << "Welcome\n";
    
    Cube8* cube = new Cube8();
    
    /*Cube8TextRenderer* textRenderer = new Cube8TextRenderer();
    cube->shareWithRenderer(textRenderer);*/
    
    Cube8VisualRenderer* visualRenderer = new Cube8VisualRenderer(" visual", 640, 480);
    cube->shareWithRenderer(visualRenderer);
    
    signed int* inputs = (signed int*)malloc(16*sizeof(signed int));
    signed int* lastInputs = (signed int*)malloc(16*sizeof(signed int));
    
    const unsigned char* keyboardState = visualRenderer->getKeyboardState();
    
    
    Game* game = new _2048Game(cube, inputs, lastInputs);
    
    
    //textRenderer->draw();
    
    while(visualRenderer->isRunning() && game->isRunning()){
        for(int i = 0; i < 16; i++){
            lastInputs[i] = inputs[i];
        }
        
        inputs[INPUT_START] = keyboardState[VISUAL_KEY_CODE_START]?1:0;
        inputs[INPUT_SELECT] = keyboardState[VISUAL_KEY_CODE_SELECT]?1:0;
        inputs[INPUT_LEFT] = keyboardState[VISUAL_KEY_CODE_LEFT]?1:0;
        inputs[INPUT_RIGHT] = keyboardState[VISUAL_KEY_CODE_RIGHT]?1:0;
        inputs[INPUT_FORWARD] = keyboardState[VISUAL_KEY_CODE_FORWARD]?1:0;
        inputs[INPUT_BACKWARD] = keyboardState[VISUAL_KEY_CODE_BACKWARD]?1:0;
        inputs[INPUT_CIRCLE] = keyboardState[VISUAL_KEY_CODE_A]?1:0;
        inputs[INPUT_CROSS] = keyboardState[VISUAL_KEY_CODE_B]?1:0;
        inputs[INPUT_TRIANGLE] = keyboardState[VISUAL_KEY_CODE_X]?1:0;
        inputs[INPUT_SQUARE] = keyboardState[VISUAL_KEY_CODE_Y]?1:0;
        inputs[INPUT_L1] = keyboardState[VISUAL_KEY_CODE_L1]?1:0;
        inputs[INPUT_R1] = keyboardState[VISUAL_KEY_CODE_R1]?1:0;
        inputs[INPUT_L2] = keyboardState[VISUAL_KEY_CODE_L2]?1:0;
        inputs[INPUT_R2] = keyboardState[VISUAL_KEY_CODE_R2]?1:0;
        inputs[INPUT_LS] = keyboardState[VISUAL_KEY_CODE_LS]?1:0;
        inputs[INPUT_RS] = keyboardState[VISUAL_KEY_CODE_RS]?1:0;
        
        game->update(visualRenderer->getDelta());
        visualRenderer->draw();
    }
    
    
    
    free(inputs);
    free(lastInputs);
    
    delete game;
    
    //delete textRenderer;
    delete visualRenderer;
    
    delete cube;
    
    return 0;
}
