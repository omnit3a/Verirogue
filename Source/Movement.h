#ifndef MOVEMENT_H_
#define MOVEMENT_H_

extern char biome;

extern int isSwimming;

extern int surroundingTemperature;

extern int entryX, entryY;

extern int turn;

extern int ch;

void checkForStairway();

void drawPlayer();

void getMovement();

void updateTemperature();

#endif
