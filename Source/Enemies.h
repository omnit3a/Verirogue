#include "Components.h"
#include <string>

#ifndef ENEMIES_H_
#define ENEMIES_H_

extern int enemyDiseaseMap[][80];	//contains wether or not an enemy is diseaseod
extern char enemyMap[][80];		//contains the locations of every creature in a dungeon
extern int enemyHealthMap[][80];	//contains the health of every creature in a dungeon
extern int enemyFleeingMap[][80]; 	//contains whether or not an enemy should be fleeing
extern std::string enemyNamesMap[][80];	//contains the names of every enemy
extern int targetingPlayerMap[][80];	//contains whether or not a specific enemy is attacking the player
extern int enemyCivlizationMap[][80];	//contains the civilization that an enemy is from

extern int spawnRate, baseEnemyHealth, baseEnemyDamage, diseaseRate, diseaseSpreadRate;

extern char goblinConsonants[8];
extern char goblinVowels[3];

std::string generateGoblinName(int xPos, int yPos);

void placeEnemies();			//generates and places enemies for use in a dungeon

void resetEnemies(int resetCode);	//removes all enemies from the map

int shouldFlee(int checkX, int checkY);

void pseudoPathfind();			//very basic pathfinding for the enemies

void updateEnemyHealth();

void targetPlayer(int xPos, int yPos);

#endif
