#!/usr/bin/env bash

echo Running Sixpair
sudo sixpair

echo Starting Sixad
sudo sixad -s

echo Starting Menu
./menu
echo Menu Closed

echo Turning off lights
./lightsOff
