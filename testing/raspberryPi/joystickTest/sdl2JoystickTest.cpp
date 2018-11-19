

#include <SDL2/SDL.h>

// g++ sdl2JoystickTest.cpp `sdl2-config --cflags --libs` -o sdl2JoystickTest


int main(){

    if(SDL_Init(SDL_INIT_JOYSTICK) < 0) {
        printf("SDL init failed: %s\n", SDL_GetError());
        return 1;
    }

    SDL_JoystickEventState(SDL_DISABLE);

    int joystickCount = SDL_NumJoysticks();

    printf("Number of joysticks found: %d\n", joystickCount);

    SDL_Joystick* controllers[joystickCount];

    for (int i = 0; i < joystickCount; i++) {
        controllers[i] = SDL_JoystickOpen(i);
        if (!controllers[i]) {
            printf( "Unable to open controller %d, SDL Error: %s\n", i, SDL_GetError());
        }
    }

    SDL_Event e;

    bool running = true;

    while(running){

        while(SDL_PollEvent(&e)){
            if(e.type == SDL_QUIT){
                printf("quit\n");
                running = false;
            }
        }

        SDL_JoystickUpdate();

        for(int i = 0; i < joystickCount; i++){
            int axes = SDL_JoystickNumAxes(controllers[i]);
            for(int j = 0; j < axes; j++){
                int value = SDL_JoystickGetAxis(controllers[i], j);
                printf("Axis %d: %d\n", j, value);
            }
        }
        
    }

    return 0;
}