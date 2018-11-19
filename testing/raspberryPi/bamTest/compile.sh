#! /bin/bash
g++ -O3 -pthread -lwiringPi $1.cpp -o $1
