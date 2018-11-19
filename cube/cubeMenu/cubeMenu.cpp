
#include <stdlib.h>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <dirent.h>
#include <stdio.h>
#include <iostream>

#include "../cubeCore/cubeCore.hpp"

struct Frame {
    double delay;
    color_t buffer[512];
};

struct Anim {
    std::string name;
    std::vector<Frame> frames;
};

bool loadAnim(std::string s, Anim& anim){

    printf("Loading Anim: %s\n", s.c_str());

    anim.name = s.substr(0, s.size()-5);
    
    Frame frame;

    std::map<char, color_t> colorMap;

    std::ifstream infile(("games/"+s).c_str());
    
    bool lookingForVar = true;
    bool lookingForLayers = false;
    int lookingAtLayerLeft = 0;
    char layerT;
    int layerI;

    std::string line;
    while(std::getline(infile, line)){
        //printf("Line: %s\n", line.c_str());
        if(line[0] == '#' || line.size() == 0){
            continue;
        }
        if(lookingForVar){
            std::size_t found = line.find(":");
            std::string var = line.substr(0, found);
            if(lookingForLayers){
                if(var.compare("xLayer") == 0){
                    //printf("xLayer\n");
                    layerT = 'x';
                    layerI = stoi(line.substr(found+1));
                    lookingForVar = false;
                    lookingAtLayerLeft = 8;
                }else if(var.compare("yLayer") == 0){
                    //printf("yLayer\n");
                    layerT = 'y';
                    layerI = stoi(line.substr(found+1));
                    lookingForVar = false;
                    lookingAtLayerLeft = 8;
                }else if(var.compare("zLayer") == 0){
                    //printf("zLayer\n");
                    layerT = 'z';
                    layerI = stoi(line.substr(found+1));
                    lookingForVar = false;
                    lookingAtLayerLeft = 8;
                }else if(var.compare("set") == 0){
                    int n = stoi(line.substr(found+1, 4));
                    int p = COORD_8(n/100, (n/10)%10, n%10);
                    color_t c = colorMap[line[found+5]];
                    frame.buffer[p] = c;
                }else if(var.compare("endframe") == 0){
                    lookingForVar = true;
                    lookingForLayers = false;
                    anim.frames.push_back(frame);
                    //printf("Added Frame\n");
                }
            }else{
                if(var.compare("frame") == 0){
                    lookingForLayers = true;
                    for(int i = 0; i < 512; i++){
                        frame.buffer[i] = 0;
                    }
                    frame.delay = stoi(line.substr(found+1))/1000.0;
                }else if(var.compare("frameD") == 0){
                    lookingForLayers = true;
                    // keep frame buffer the same as previous
                    frame.delay = stoi(line.substr(found+1))/1000.0;
                }else if(var.size() == 1){
                    char c = var[0];
                    colorMap.emplace(c, (color_t)std::stoul(line.substr(found+1).c_str(), 0, 16));
                    //printf("%c: %X\n", c, colorMap[c]);
                }else{
                    printf("Unknown var \'%s\'\n", var.c_str());
                    printf("Failed to load\n");
                    return false;
                }
            }
        } else {
            if(lookingAtLayerLeft > 0){
                //printf("layerLine\n");
                int x, y, z;
                if(layerT == 'x'){
                    x = layerI;
                    y = 0;
                    z = 8-lookingAtLayerLeft;
                }
                if(layerT == 'y'){
                    x = 0;
                    y = layerI;
                    z = 8-lookingAtLayerLeft;
                }
                if(layerT == 'z'){
                    x = 0;
                    y = 8-lookingAtLayerLeft;
                    z = layerI;
                }
                for(int i = 0; i < line.size(); i++){
                    color_t c;
                    if(line[i] == ' '){
                        c = 0;
                    }else{
                        if(colorMap.count(line[i])){
                            c = colorMap[line[i]];
                        }else{
                            printf("Unknown color \'%c\', defaulting with off\n", line[i]);
                            c = 0;
                        }
                    }
                    frame.buffer[COORD_8(x, y, z)] = c;
                    if(layerT == 'x')y++;
                    if(layerT == 'y')x++;
                    if(layerT == 'z')x++;
                }
                lookingAtLayerLeft--;
                //printf("lookingAtLayerLeft: %d\n", lookingAtLayerLeft);
            }
            if(lookingAtLayerLeft == 0){
                lookingForVar = true;
                lookingForLayers = true;
            }
        }

    }

    printf("Loaded\n");

    return true;
}

std::vector<Anim> loadAnims(){
    std::vector<Anim> anims;
    DIR *dp;
    struct dirent *dirp;
    dp = opendir("games");

    while ((dirp = readdir(dp)) != NULL) {
        std::string fn = std::string(dirp->d_name);
        if(fn.size() > 5){
            if(fn.compare(fn.size()-5, 5, ".anim") == 0){
                Anim anim;
                if(loadAnim(fn.c_str(), anim)){
                    anims.push_back(anim);
                }
            }
        }
    }
    closedir(dp);
    return anims;
}

int selected = 0;

std::string runMenu(){

    while(!JoystickCore::start()){
        printf("Waiting for joystick core...\n");
        usleep(500000);
    }
    printf("Joystick core started, starting menu.\n");
    
    LightCore::start();

    std::vector<Anim> anims = loadAnims();

    for(int i = 0; i < anims.size(); i++){
        printf("Game [%s]: anim has %lu frames\n", anims[i].name.c_str(), anims[i].frames.size());
        /*for(int j = 0; j < anims[i].frames.size(); j++){
            printf("Frame %d\n", j);
            for(int k = 0; k < 512; k++){
                printf("%X ", anims[i].frames[j].buffer[k]);
            }
            printf("\n");
        }*/
    }

    if(selected >= anims.size()){
        selected = 0;
    }

    // load all *.anim files in games/
    // they will be animation files that are the menu logos

    bool running = true;

    bool dirty = true;

    clock_t begin = clock();
    int frame = 0;
    double timer = 0;

    while(running){
        JoystickCore::processEvents();


        if(timer >= anims[selected].frames[frame].delay){
            timer -= anims[selected].frames[frame].delay;

            frame = (frame+1)%anims[selected].frames.size();
            dirty = true;
        }

        if(dirty){
            LightCore::clearAll();
            LightCore::setLights(anims[selected].frames[frame].buffer);
            LightCore::swapBuffers();
            dirty = false;
        }

        clock_t end = clock();
        double delta = double(end - begin) / CLOCKS_PER_SEC;
        begin = end;

        timer += delta;

        if(JoystickCore::buttonPressed[JS_BUTTON_START] || JoystickCore::buttonPressed[JS_BUTTON_A]){
            running = false;
            printf("Selected game, exiting menu.\n");
        }
        if(JoystickCore::buttonPressed[JS_BUTTON_LEFT]){
            selected = selected==0?(anims.size()-1):(selected-1);
            frame = 0;
            timer = 0;
            dirty = true;
            printf("Left, selecting index %d.\n", selected);
        }
        if(JoystickCore::buttonPressed[JS_BUTTON_RIGHT]){
            selected = (selected+1)%anims.size();
            frame = 0;
            timer = 0;
            dirty = true;
            printf("Right, selecting index %d.\n", selected);
        }


        //running = false; // temp
    }

    JoystickCore::stop();
    LightCore::stop();

    return anims[selected].name;

}

int main(){

    #ifdef __arm__
    system("sudo sixpair");
    system("sudo sixad -s");
    #endif

    while(true){
        std::string selectedGame = runMenu();
        system(("./games/"+selectedGame).c_str());
    }

    return 0;
}