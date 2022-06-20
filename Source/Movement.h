#ifndef MOVEMENT_H_
#define MOVEMENT_H_

extern char biome;

extern int isSwimming, onFire, hasHypothermia, hasFever;

extern int surroundingTemperature;

extern int entryX, entryY, fires;

extern int turn;

extern int ch;

extern char dir;

void drawPlayer();

int townWalkable(int x, int y);

void getMovement();

void updateTemperature();

void updateHealth();

void updateHunger();

#endif
