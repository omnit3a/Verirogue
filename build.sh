#!/bin/bash

sudo cp -r -p Source/. Build
cd Build
g++ -o Main Main.c MapGenerator.c DrawScreen.c Player.c Movement.c DrawUI.c PlayerCreator.c Planets.c Enemies.c Inventory.c Items.c FileRead.c Gas.c Legacy.c -lm -lncurses -L../Source/FastNoiseLite.h
./Main
find . -type f -name '*.c' -delete
find . -type f -name '*.h' -delete
