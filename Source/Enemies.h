#include "Components.h"

#ifndef ENEMIES_H_
#define ENEMIES_H_

extern int enemyDiseaseMap[][80];	//contains wether or not an enemy is diseaseod
extern char enemyMap[][80];		//contains the locations of every creature in a dungeon
extern int enemyHealthMap[][80];	//contains the health of every creature in a dungeon

void placeEnemies();			//generates and places enemies for use in a dungeon

void resetEnemies();			//removes all enemies from the map

void pseudoPathfind();			//very basic pathfinding for the enemies

void updateEnemyHealth();

void targetPlayer();

#endif
