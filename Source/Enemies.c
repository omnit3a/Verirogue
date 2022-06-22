#include <stdio.h>
#include <stdlib.h>
#include "Components.h"
#include "Enemies.h"
#include "MapGenerator.h"
#include "Movement.h"
#include "Player.h"

char enemyMap[24][80];
int enemyHealthMap[24][80];

void placeEnemies(){
	if (biome == 'd'){
		for (int i = 0 ; i < 80 ; i++){
			for (int j = 0 ; j < 24 ; j++){
				enemyMap[j][i] = ' ';
				enemyHealthMap[j][i] = 0;
				if (map[j][i] == '.'){
					if (rand() % 45 == 0){
						enemyMap[j][i] = '&';
						enemyHealthMap[j][i] = 50;
					}
				}
			}
		}
	}
}

void resetEnemies(){
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			enemyMap[j][i] = ' ';
			enemyHealthMap[j][i] = 0;
		}
	}
}

void pseudoPathfind(){
	int checkX = playerEnt.currentPos.xPos;
	int checkY = playerEnt.currentPos.yPos;
	char tempMap[24][80];
	int tempHealth[24][80];
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			tempMap[j][i] = enemyMap[j][i];
			tempHealth[j][i] = enemyHealthMap[j][i];
		}
	}
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			if (enemyMap[j][i] == '&'){
				if (map[j+1][i] != ' ' && j < checkY-1 && enemyMap[j+1][i] != '&'){
					tempMap[j][i] = ' ';
					tempMap[j+1][i] = '&';
					tempHealth[j+1][i] = tempHealth[j][i];
					tempHealth[j][i] = 0;
				} else if (map[j-1][i] != ' ' && j > checkY+1 && enemyMap[j-1][i] != '&'){
					tempMap[j][i] = ' ';
					tempMap[j-1][i] = '&';
					tempHealth[j-1][i] = tempHealth[j][i];
					tempHealth[j][i] = 0;
				} else if (map[j][i+1] != ' ' && i < checkX-1 && enemyMap[j][i+1] != '&'){
					tempMap[j][i] = ' ';
					tempMap[j][i+1] = '&';
					tempHealth[j][i+1] = tempHealth[j][i];
					tempHealth[j][i] = 0;
				} else if (map[j][i-1] != ' ' && i > checkX+1 && enemyMap[j][i-1] != '&'){
					tempMap[j][i] = ' ';
					tempMap[j][i-1] = '&';
					tempHealth[j][i-1] = tempHealth[j][i];
					tempHealth[j][i] = 0;
				}
			}
		}
	}
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			enemyMap[j][i] = tempMap[j][i];
			enemyHealthMap[j][i] = tempHealth[j][i];
		}
	}
}
