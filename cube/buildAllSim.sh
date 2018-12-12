#!/usr/bin/env bash

./buildGameSim.sh TicTacToe &
./buildGameSim.sh Snake &
./buildGameSim.sh Reversi &
./buildGameSim.sh Effects &
#./buildGameSim.sh 2PSnake &
#./buildGameSim.sh Fludd &
./buildGameSim.sh PuzzleCube &
./buildGameSim.sh Connect4 &

g++ -std=c++11 cubeMenu/cubeMenu.cpp cubeCore/cubeCore.cpp cubeCore/geometry.cpp cubeCore/color.cpp cubeCore/perlin.cpp cubeCore/visualRenderer.cpp `pkg-config --cflags --libs sdl2` -o menu