#!/usr/bin/env bash

g++ -lwiringPi -pthread -std=c++11 cubeMenu/cubeMenu.cpp cubeCore/cubeCore.cpp cubeCore/geometry.cpp -o menu

./buildGamePi.sh TicTacToe
./buildGamePi.sh Snake
./buildGamePi.sh Reversi