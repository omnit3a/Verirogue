#!/bin/bash

clear
sudo cp -r -p Source/. Build
cd Build
# you can replace "Linux-Verirogue" with whatever you want, I just have it this way cause I develop the game on linux
g++ -o Linux-Verirogue Main.c MapGenerator.c DrawScreen.c Player.c Movement.c DrawUI.c PlayerCreator.c Planets.c Enemies.c Inventory.c Items.c FileRead.c Gas.c Legacy.c Utils.c History.c -lm -lncurses -L../Source/FastNoiseLite.h
./Linux-Verirogue
find . -type f -name '*.c' -delete
find . -type f -name '*.h' -delete
