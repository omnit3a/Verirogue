#ifndef PLAYER_H_
#define PLAYER_H_

#include "Components.h"

extern int foodCount;

extern int foodScore;

extern int goldScore;

extern int currentWeapon;

extern Humanoid playerEnt;

void setupPlayer(int x, int y, int entID);

void killCheck();

#endif

