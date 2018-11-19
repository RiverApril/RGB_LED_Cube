#!/usr/bin/env bash

g++ -lwiringPi -pthread -std=c++11 games/$1.cpp cubeCore/cubeCore.cpp cubeCore/geometry.cpp -o games/$1