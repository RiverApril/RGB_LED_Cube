#!/usr/bin/env bash

g++ -std=c++11 games/$1.cpp cubeCore/cubeCore.cpp cubeCore/geometry.cpp cubeCore/color.cpp cubeCore/perlin.cpp cubeCore/visualRenderer.cpp `pkg-config --cflags --libs sdl2` -o games/$1 && echo $1 Built