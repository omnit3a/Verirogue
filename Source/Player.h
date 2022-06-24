#define DEADLYBLOODLOSS 255

#ifndef PLAYER_H_
#define PLAYER_H_

#include "Components.h"

extern int bloodCount;

extern int bloodLossRate;

extern int foodCount;

extern int foodScore;

extern int goldScore;

extern int isInfected, hasDiarrhea, isCongested, hasRash;

extern int currentWeapon;

extern Humanoid playerEnt;

void setupPlayer(int x, int y, int entID);

void killCheck();

void bleedCheck();

void infectionCheck();

#endif

