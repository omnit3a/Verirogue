#define DEADLYBLOODLOSS 255
#include <string>


#ifndef PLAYER_H_
#define PLAYER_H_

#include "Components.h"

extern int bloodCount;

extern int bloodLossRate;

extern int foodCount;

extern int foodScore;

extern int goldScore;

extern int isInfected, hasDiarrhea, isCongested, hasRash, infectionStart, isDiseased, isCrying;

extern int diseaseStart;

extern int currentWeapon;

extern Humanoid playerEnt;

extern int isSneaking;

extern int bloodMap[][80];

extern int grossStuff;

extern std::string playerName;

void setupPlayer(int x, int y, int entID);

void killPlayer(std::string text);

void cryCheck();

void killCheck();

void bloodTrail();

void bleedCheck();

void infectionCheck();

void cauterizeWound();

#endif

