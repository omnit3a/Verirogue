#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "Components.h"
#include "Enemies.h"
#include "MapGenerator.h"
#include "Movement.h"
#include "Player.h"
#include "DrawUI.h"

int enemyDiseaseMap[24][80];
char enemyMap[24][80];
int enemyHealthMap[24][80];

int spawnRate = 45;
int baseEnemyHealth = 10;
int baseEnemyDamage = 1;
int diseaseRate = 10;
int diseaseSpreadRate = 10;

void placeEnemies(){
	if (biome == 'd'){
		for (int i = 0 ; i < 80 ; i++){
			for (int j = 0 ; j < 24 ; j++){
				enemyMap[j][i] = ' ';
				enemyHealthMap[j][i] = 0;
				if (map[j][i] == '.' && rand() % spawnRate == 0){
					enemyMap[j][i] = '&';
					enemyHealthMap[j][i] = (rand() % 10)+baseEnemyHealth;
					if (rand() % diseaseRate == 0){
						enemyDiseaseMap[j][i] = 1;
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
			enemyDiseaseMap[j][i] = 0;
		}
	}
}

void pseudoPathfind(){
	if (!isSneaking){
		int checkX = playerEnt.currentPos.xPos;
		int checkY = playerEnt.currentPos.yPos;
		char tempMap[24][80];
		int tempHealth[24][80];
		int tempDisease[24][80];
		for (int i = 0 ; i < 80 ; i++){
			for (int j = 0 ; j < 24 ; j++){
				tempMap[j][i] = enemyMap[j][i];
				tempHealth[j][i] = enemyHealthMap[j][i];
				tempDisease[j][i] = enemyDiseaseMap[j][i];
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
						tempDisease[j+1][i] = tempDisease[j][i];
						tempDisease[j][i] = 0;
					} else if (map[j-1][i] != ' ' && j > checkY+1 && enemyMap[j-1][i] != '&'){
						tempMap[j][i] = ' ';
						tempMap[j-1][i] = '&';
						tempHealth[j-1][i] = tempHealth[j][i];
						tempHealth[j][i] = 0;
						tempDisease[j-1][i] = tempDisease[j][i];
						tempDisease[j][i] = 0;
					} else if (map[j][i+1] != ' ' && i < checkX-1 && enemyMap[j][i+1] != '&'){
						tempMap[j][i] = ' ';
						tempMap[j][i+1] = '&';
						tempHealth[j][i+1] = tempHealth[j][i];
						tempHealth[j][i] = 0;
						tempDisease[j][i+1] = tempDisease[j][i];
						tempDisease[j][i] = 0;
					} else if (map[j][i-1] != ' ' && i > checkX+1 && enemyMap[j][i-1] != '&'){
						tempMap[j][i] = ' ';
						tempMap[j][i-1] = '&';
						tempHealth[j][i-1] = tempHealth[j][i];
						tempHealth[j][i] = 0;
						tempDisease[j][i-1] = tempDisease[j][i];
						tempDisease[j][i] = 0;
					}
				}
			}
		}
		for (int i = 0 ; i < 80 ; i++){
			for (int j = 0 ; j < 24 ; j++){
				enemyMap[j][i] = tempMap[j][i];
				enemyHealthMap[j][i] = tempHealth[j][i];
				enemyDiseaseMap[j][i] = tempDisease[j][i];
			}
		}	
	}
}

void updateEnemyHealth(){
	srand(time(0));
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			if (enemyHealthMap[j][i] <= 0 && enemyMap[j][i] != ' '){
				enemyMap[j][i] = ' ';
				goldScore += (rand() % 10) + 1;
				msgLog = "The enemy died!";
			}
		}
	}
}

void targetPlayer(){
	srand(time(0));
	int target = rand() % 6;
	int damage = (rand() % 7)+baseEnemyDamage;
	if (rand() % 3 < 2){
		switch (target){
			case 0:
				playerEnt.head.bpHP.currentHealth -= damage;
				if (playerEnt.head.bpHP.currentHealth <=0){
					playerEnt.head.bpHP.currentHealth = 0;
				}
				msgLog = "You were hit in the head!";
				break;
			case 1:
				playerEnt.torso.bpHP.currentHealth -= damage;
				if (playerEnt.torso.bpHP.currentHealth <= 0){
					playerEnt.torso.bpHP.currentHealth = 0;
				}
				msgLog = "You were hit in the torso!";
				break;
			case 2:
				playerEnt.leftArm.bpHP.currentHealth -= damage;
				if (playerEnt.leftArm.bpHP.currentHealth <= 0){
					playerEnt.leftArm.bpHP.currentHealth = 0;
				}
				msgLog = "Your left arm was hit!";
				break;
			case 3:
				playerEnt.rightArm.bpHP.currentHealth -= damage;
				if (playerEnt.rightArm.bpHP.currentHealth <= 0){
					playerEnt.rightArm.bpHP.currentHealth = 0;
				}
				msgLog = "Your right arm was hit!";
				break;
			case 4:
				playerEnt.leftLeg.bpHP.currentHealth -= damage;
				if (playerEnt.leftLeg.bpHP.currentHealth <= 0){
					playerEnt.leftLeg.bpHP.currentHealth = 0;
				}
				msgLog = "Your left leg was hit!";
				break;
			case 5:
				playerEnt.rightLeg.bpHP.currentHealth -= damage;
				if (playerEnt.rightLeg.bpHP.currentHealth <= 0){
					playerEnt.rightLeg.bpHP.currentHealth = 0;
				}
				msgLog = "Your right leg was hit!";
				break;
		}
		playerEnt.skin.bpHP.currentHealth--;
		srand(time(0)+playerEnt.skin.bpHP.currentHealth);
		if (rand() % diseaseSpreadRate == 0){
			msgLog = "You contract a disease!";
			isDiseased = 1;
			diseaseStart = turn;
		}
	}
	
}
