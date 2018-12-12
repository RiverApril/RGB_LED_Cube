

#include "../cubeCore/cubeCore.hpp"

#include <time.h>

#define COLOR_1 0xFF0000
#define COLOR_2 0x0000FF
#define COLOR_BLINK_1 0xFF8080
#define COLOR_BLINK_2 0x8080FF

unsigned char marks[64];
unsigned char winnerMarks[64];

void clearWinnerMarks(){
    for(pos_t i = 0; i < 64; i++){
        winnerMarks[i] = 0;
    }
}

void checkWin(unsigned char& winner){
    // check in z
    for(unsigned char x = 0; !winner && x < 4; x++){
        for(unsigned char y = 0; !winner && y < 4; y++){
            clearWinnerMarks();
            unsigned char a = marks[COORD_4(x, y, 0)];
            if(a != 0){
                winnerMarks[COORD_4(x, y, 0)] = a;
                for(unsigned char z = 1; z < 4; z++){
                    if(marks[COORD_4(x, y, z)] != a){
                        a = 0;
                        break;
                    }else{
                        winnerMarks[COORD_4(x, y, z)] = a;
                    }
                }
                if(a != 0){
                    winner = a;
                }
            }
        }
    }
    // check in y
    for(unsigned char x = 0; !winner && x < 4; x++){
        for(unsigned char z = 0; !winner && z < 4; z++){
            clearWinnerMarks();
            unsigned char a = marks[COORD_4(x, 0, z)];
            if(a != 0){
                winnerMarks[COORD_4(x, 0, z)] = a;
                for(unsigned char y = 1; y < 4; y++){
                    if(marks[COORD_4(x, y, z)] != a){
                        a = 0;
                        break;
                    }else{
                        winnerMarks[COORD_4(x, y, z)] = a;
                    }
                }
                if(a != 0){
                    winner = a;
                }
            }
        }
    }
    // check in x
    for(unsigned char y = 0; !winner && y < 4; y++){
        for(unsigned char z = 0; !winner && z < 4; z++){
            clearWinnerMarks();
            unsigned char a = marks[COORD_4(0, y, z)];
            if(a != 0){
                winnerMarks[COORD_4(0, y, z)] = a;
                for(unsigned char x = 1; x < 4; x++){
                    if(marks[COORD_4(x, y, z)] != a){
                        a = 0;
                        break;
                    }else{
                        winnerMarks[COORD_4(x, y, z)] = a;
                    }
                }
                if(a != 0){
                    winner = a;
                }
            }
        }
    }
    // check in yz
    for(unsigned char x = 0; !winner && x < 4; x++){
        clearWinnerMarks();
        unsigned char a = marks[COORD_4(x, 0, 0)];
        winnerMarks[COORD_4(x, 0, 0)] = a;
        for(unsigned char i = 1; i < 4; i++){
            if(a != marks[COORD_4(x, i, i)]){
                a = 0;
            }else{
                winnerMarks[COORD_4(x, i, i)] = a;
            }
        }
        if(a != 0){
            winner = a;
        }
        if(!winner){
            clearWinnerMarks();
            unsigned char a = marks[COORD_4(x, 3-0, 0)];
            winnerMarks[COORD_4(x, 3-0, 0)] = a;
            for(unsigned char i = 1; i < 4; i++){
                if(a != marks[COORD_4(x, 3-i, i)]){
                    a = 0;
                }else{
                    winnerMarks[COORD_4(x, 3-i, i)] = a;
                }
            }
            if(a != 0){
                winner = a;
            }
        }
    }
    // check in xy
    for(unsigned char z = 0; !winner && z < 4; z++){
        clearWinnerMarks();
        unsigned char a = marks[COORD_4(0, 0, z)];
        winnerMarks[COORD_4(0, 0, z)] = a;
        for(unsigned char i = 1; i < 4; i++){
            if(a != marks[COORD_4(i, i, z)]){
                a = 0;
            }else{
                winnerMarks[COORD_4(i, i, z)] = a;
            }
        }
        if(a != 0){
            winner = a;
        }
        if(!winner){
            clearWinnerMarks();
            unsigned char a = marks[COORD_4(3-0, 0, z)];
            winnerMarks[COORD_4(3-0, 0, z)] = a;
            for(unsigned char i = 1; i < 4; i++){
                if(a != marks[COORD_4(3-i, i, z)]){
                    a = 0;
                }else{
                    winnerMarks[COORD_4(3-i, i, z)] = a;
                }
            }
            if(a != 0){
                winner = a;
            }
        }
    }
    // check in xz
    for(unsigned char y = 0; !winner && y < 4; y++){
        clearWinnerMarks();
        unsigned char a = marks[COORD_4(0, y, 0)];
        winnerMarks[COORD_4(0, y, 0)] = a;
        for(unsigned char i = 1; i < 4; i++){
            if(a != marks[COORD_4(i, y, i)]){
                a = 0;
            }else{
                winnerMarks[COORD_4(i, y, i)] = a;
            }
        }
        if(a != 0){
            winner = a;
        }
        if(!winner){
            clearWinnerMarks();
            unsigned char a = marks[COORD_4(3-0, y, 0)];
            winnerMarks[COORD_4(3-0, y, 0)] = a;
            for(unsigned char i = 1; i < 4; i++){
                if(a != marks[COORD_4(3-i, y, i)]){
                    a = 0;
                }else{
                    winnerMarks[COORD_4(3-i, y, i)] = a;
                }
            }
            if(a != 0){
                winner = a;
            }
        }
    }
    // check in xyz
    if(!winner){
        clearWinnerMarks();
        unsigned char a = marks[COORD_4(0, 0, 0)];
        winnerMarks[COORD_4(0, 0, 0)] = a;
        for(unsigned char i = 1; i < 4; i++){
            if(a != marks[COORD_4(i, i, i)]){
                a = 0;
            }else{
                winnerMarks[COORD_4(i, i, i)] = a;
            }
        }
        if(a != 0){
            winner = a;
        }
    }
    if(!winner){
        clearWinnerMarks();
        unsigned char a = marks[COORD_4(3-0, 0, 0)];
        winnerMarks[COORD_4(3-0, 0, 0)] = a;
        for(unsigned char i = 1; i < 4; i++){
            if(a != marks[COORD_4(3-i, i, i)]){
                a = 0;
            }else{
                winnerMarks[COORD_4(3-i, i, i)] = a;
            }
        }
        if(a != 0){
            winner = a;
        }
    }
    if(!winner){
        clearWinnerMarks();
        unsigned char a = marks[COORD_4(0, 3-0, 0)];
        winnerMarks[COORD_4(0, 3-0, 0)] = a;
        for(unsigned char i = 1; i < 4; i++){
            if(a != marks[COORD_4(i, 3-i, i)]){
                a = 0;
            }else{
                winnerMarks[COORD_4(i, 3-i, i)] = a;
            }
        }
        if(a != 0){
            winner = a;
        }
    }
    if(!winner){
        clearWinnerMarks();
        unsigned char a = marks[COORD_4(0, 0, 3-0)];
        winnerMarks[COORD_4(0, 0, 3-0)] = a;
        for(unsigned char i = 1; i < 4; i++){
            if(a != marks[COORD_4(i, i, 3-i)]){
                a = 0;
            }else{
                winnerMarks[COORD_4(i, i, 3-i)] = a;
            }
        }
        if(a != 0){
            winner = a;
        }
    }
}

int main(){

    JoystickCore::start();
    LightCore::start();


    for(pos_t i = 0; i < 64; i++){
        marks[i] = 0;
        winnerMarks[i] = 0;
    }

    unsigned char cursorX, cursorY, cursorZ;
    cursorX = 0;
    cursorY = 0;
    cursorZ = 0;

    bool turn = false;
    unsigned char winner = 0;
    
    const double BLINK_DELAY = 0.25;
    double blinkTimer = 0;
    bool blinkOn = false;

    bool dirty = true;

    bool running = true;

    clock_t begin = clock();

    while(running){
        JoystickCore::processEvents();

        if(JoystickCore::buttonPressed[JS_BUTTON_HOME]){
            running = false;
            break;
        }

        clock_t end = clock();
        double delta = double(end - begin) / CLOCKS_PER_SEC;
        begin = end;

        blinkTimer += delta;
        if(blinkTimer >= BLINK_DELAY){
            blinkTimer -= BLINK_DELAY;
            blinkOn = !blinkOn;
            dirty = true;
        }

        if(dirty){
            LightCore::clearAll();
            for(pos_t i = 0; i < 64; i++){
                color_t c;
                pos_t x = X_COORD_4(i);
                pos_t y = Y_COORD_4(i);
                pos_t z = Z_COORD_4(i);
                if(winner){
                    if(winnerMarks[i] != 0 && !blinkOn){
                        continue;
                    }
                    if(marks[i] != 0){
                        c = marks[i]==1?COLOR_1:COLOR_2;
                    }else{
                        continue;
                    }
                }else{
                    if(blinkOn && x == cursorX && y == cursorY && z == cursorZ){
                        c = turn?COLOR_BLINK_2:COLOR_BLINK_1;
                    }else{
                        if(marks[i] != 0){
                            c = marks[i]==1?COLOR_1:COLOR_2;
                        }else{
                            continue;
                        }
                    }
                }
                LightCore::setLight(x*2, y*2, z*2, c);
                LightCore::setLight(x*2, y*2, z*2+1, c);
                LightCore::setLight(x*2, y*2+1, z*2, c);
                LightCore::setLight(x*2, y*2+1, z*2+1, c);
                LightCore::setLight(x*2+1, y*2, z*2, c);
                LightCore::setLight(x*2+1, y*2, z*2+1, c);
                LightCore::setLight(x*2+1, y*2+1, z*2, c);
                LightCore::setLight(x*2+1, y*2+1, z*2+1, c);
            }
            LightCore::swapBuffers();
            dirty = false;
        }

        if(winner == 0){
        
            bool moved = false;

            if(JoystickCore::buttonPressed[JS_BUTTON_LEFT]) { cursorX = (cursorX-1)&3; moved = true;}
            if(JoystickCore::buttonPressed[JS_BUTTON_RIGHT]){ cursorX = (cursorX+1)&3; moved = true;}
            if(JoystickCore::buttonPressed[JS_BUTTON_UP])   { cursorY = (cursorY-1)&3; moved = true;}
            if(JoystickCore::buttonPressed[JS_BUTTON_DOWN]) { cursorY = (cursorY+1)&3; moved = true;}
            if(JoystickCore::buttonPressed[JS_BUTTON_L1])   { cursorZ = (cursorZ-1)&3; moved = true;}
            if(JoystickCore::buttonPressed[JS_BUTTON_R1])   { cursorZ = (cursorZ+1)&3; moved = true;}

            if(moved){
                dirty = true;
                blinkOn = true;
                blinkTimer = 0;
            }

            if(JoystickCore::buttonPressed[JS_BUTTON_A]){
                pos_t i = COORD_4(cursorX, cursorY, cursorZ);
                if(marks[i] == 0){
                    marks[i] = turn?2:1;
                    turn = !turn;
                    dirty = true;
                    checkWin(winner);
                }
            }
        }else{
            if(JoystickCore::buttonPressed[JS_BUTTON_START]){
                
                for(pos_t i = 0; i < 64; i++){
                    marks[i] = 0;
                    winnerMarks[i] = 0;
                }
                turn = false;
                winner = 0;

            }
        }

    }

    JoystickCore::stop();
    LightCore::stop();

    return 0;
}
