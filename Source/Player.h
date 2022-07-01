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

extern int grossStuff, bloodTrailThreshold, DEADLYBLOODLOSS;

extern std::string playerName, playerRace;
extern std::string enemyNamesList[512];
extern std::string killedByEnemy;

void setupPlayer(int x, int y, int entID);

void killPlayer(std::string text);

void cryCheck();

void killCheck();

void bloodTrail();

void bleedCheck();

void infectionCheck();

void cauterizeWound();

#endif

