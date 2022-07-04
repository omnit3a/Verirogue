#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <cctype>
#include "Components.h"
#include "Enemies.h"
#include "MapGenerator.h"
#include "Movement.h"
#include "Player.h"
#include "DrawUI.h"
#include "Legacy.h"
#include "Main.h"
#include "History.h"
#include "Planets.h"
#include "DrawScreen.h"

int enemyDiseaseMap[24][80];
char enemyMap[24][80];
int enemyHealthMap[24][80];
int enemyFleeingMap[24][80];
std::string enemyNamesMap[24][80];
int targetingPlayerMap[24][80];
int enemyCivilizationMap[24][80];
char enemyHandednessMap[24][80];

int spawnRate = 45;
int baseEnemyHealth = 10;
int baseEnemyDamage = 1;
int diseaseRate = 10;
int diseaseSpreadRate = 10;

char goblinConsonants[8] = {'x','h','g','z','t','k','q','w'};
char goblinVowels[5] = {'a','o','e','u','i'};

char koboldOnset[5] = {'t','l','h','n','j'};
char koboldNucleas[8] = {'y','i','l','u','e','a','*','`'};
char koboldCoda[5] = {'r','n','v','d','s'};

std::string generateGoblinName(int xPos, int yPos){
	std::string tempString = "";
	int pairs = (rand() % 4)+1;
	int consonantValue;
	int vowelValue;
	char syllablePair[2];
	int currentCiv = enemyCivilizationMap[yPos][xPos];
	for (int i = 0 ; i < pairs ; i++){
		consonantValue = rand() % 8;
		vowelValue = rand() % 5;
		if (i == 0){
			syllablePair[0] = goblinConsonants[consonantValue] - 32;
		} else {
			syllablePair[0] = goblinConsonants[consonantValue];
		}
		syllablePair[1] = goblinVowels[vowelValue];
		tempString = tempString+syllablePair;
		if (rand() % 4 == 0){
			if (rand() % 2 == 0){
				tempString = tempString+"m";
			} else {
				tempString = tempString+"n";
			}
		}
	}
	tempString = tempString+" the Goblin of "+civilizations[currentCiv];
	return tempString;
	
}

std::string generateKoboldName(){
	std::string tempString = "";
	int pairs = (rand() % 2)+1;
	int onsetValue;
	int onsetAmount;
	int nucleasValue;
	int codaValue;
	int codaAmount;
	for (int i = 0 ; i < pairs ; i++){
		onsetAmount = (rand() % 2)+1;
		codaAmount = (rand() % 2)+1;
		for (int o = 0 ; o < onsetAmount ; o++){
			onsetValue = rand() % 5;
			if (i == 0 && o == 0){
				tempString = tempString + koboldOnset[onsetValue];
			} else {
				tempString = tempString + koboldOnset[onsetValue];
			}
		}
		nucleasValue = rand() % 8;
		tempString = tempString + koboldNucleas[nucleasValue];
		for (int c = 0 ; c < codaAmount ; c++){
			codaValue = rand() % 5;
			tempString = tempString + koboldCoda[codaValue];
		}
	}
	tempString = tempString + " the Kobold";
	tempString[0] = std::toupper(tempString[0]);
	return tempString;
	
}

std::string whichEnemy(char enemy){
	switch (enemy){
		case GOBLINCHAR:
			return "Goblin";
			break;
		case KOBOLDCHAR:
			return "Kobold";
			break;
		default:
			return "Nothing";
			break;
	}
}

void placeEnemies(){
	if (biome == 'd'){
		int currentCiv;
		int tempSeed = 0;
		int tempCiv;
		int handedness;
		for (int i = 0 ; i < 80 ; i++){
			for (int j = 0 ; j < 24 ; j++){
				handedness = rand() % 100;
				if (handedness > 10){
					enemyHandednessMap[j][i] = 'L';
				} else {
					enemyHandednessMap[j][i] = 'R';
				}
				enemyMap[j][i] = ' ';
				enemyHealthMap[j][i] = 0;
				enemyFleeingMap[j][i] = 0;
				if ((dungeonWalkable(i,j) && map[j][i] != '<') && rand() % spawnRate == 0){
					srand(time(0)+tempSeed);
					if (rand() % 2 == 0){
						enemyMap[j][i] = GOBLINCHAR;
					} else {
						enemyMap[j][i] = KOBOLDCHAR;
					}
					enemyHealthMap[j][i] = (rand() % 10)+baseEnemyHealth;
					if (rand() % diseaseRate == 0){
						enemyDiseaseMap[j][i] = 1;
					}
					if (whichEnemy(enemyMap[j][i]) == "Kobold"){
						enemyNamesMap[j][i] = generateKoboldName();
					}
					if (whichEnemy(enemyMap[j][i]) == "Goblin"){
						enemyCivilizationMap[j][i] = rand() % 4;
					} else if (whichEnemy(enemyMap[j][i]) == "Kobold"){
						//places some kobolds in a non-racist civilization
						for (int r = 0 ; r < 2 ; r++){
							srand(time(0)+tempSeed+r);
							tempCiv = rand() % 4;
							if (!civilizationRacist[tempCiv]){
								enemyCivilizationMap[j][i] = tempCiv;
								enemyNamesMap[j][i] = enemyNamesMap[j][i]+" of "+civilizations[tempCiv];
								break;
							}
						}
					}
					if (whichEnemy(enemyMap[j][i]) == "Goblin"){
						enemyNamesMap[j][i] = generateGoblinName(i,j);
					}
					targetingPlayerMap[j][i] = 1;
					tempSeed++;
				}
			}
		}
	}
}

/**
 *	Reset code 0: delete all enemies
 *	Reset code 1: cull enemies
 */
void resetEnemies(int resetCode){
	if (resetCode == 0){
		for (int i = 0 ; i < 80 ; i++){
			for (int j = 0 ; j < 24 ; j++){
				enemyMap[j][i] = ' ';
				enemyHealthMap[j][i] = 0;
				enemyDiseaseMap[j][i] = 0;
				enemyFleeingMap[j][i] = 0;
				enemyNamesMap[j][i] = "";
				targetingPlayerMap[j][i] = 0;
				enemyCivilizationMap[j][i] = -1;
			}	
		}
	} else if (resetCode == 1){
		for (int i = 0 ; i < 80 ; i++){
			for (int j = 0 ; j < 24 ; j++){
				if (!dungeonWalkable(i,j) && enemyMap[j][i] != ' '){
					enemyMap[j][i] = ' ';
					enemyHealthMap[j][i] = 0;
					enemyDiseaseMap[j][i] = 0;
					enemyFleeingMap[j][i] = 0;
					enemyNamesMap[j][i] = "";
					targetingPlayerMap[j][i] = 0;
					enemyCivilizationMap[j][i] = -1;
				}
			}
		}
	}
}

/**
 *	Determine whether or not an enemy should flee from the player
 */
int shouldFlee(int checkX, int checkY){
	int tempFlee = 0;
	if (enemyHealthMap[checkY-1][checkX] == -1){
		tempFlee++;
	}
	if (enemyHealthMap[checkY+1][checkX] == -1){
		tempFlee++;
	}
	if (enemyHealthMap[checkY][checkX-1] == -1){
		tempFlee++;
	}
	if (enemyHealthMap[checkY][checkX+1] == -1){
		tempFlee++;
	}
	return tempFlee >= 2;
}

void targetPlayer(int xPos, int yPos){
	srand(time(0));
	int enemyStrength = 0;
	int enemyIntelligence = 0;
	int enemyCoward = 0;
	int currentCiv = enemyCivilizationMap[yPos][xPos];
	for (int i = 0 ; i < 4 ; i++){
		if (i == currentCiv && whichEnemy(enemyMap[yPos][xPos]) == "Goblin"){
			enemyCoward = civilizationPeaceful[currentCiv];
			if ((playerRace == "Kobold") && civilizationRacist[currentCiv]){
				enemyStrength = 5;
			}
			enemyIntelligence = civilizationSmart[currentCiv];
		}	
	}
	if (enemyHandednessMap[yPos][xPos] == 'R' && playerHandedness == "Left handed"){
		enemyStrength += 3;
	}
	int target = rand() % 6;
	int damage = (rand() % 7)+baseEnemyDamage+enemyStrength;
	if (!enemyCoward){
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
				diseaseStart = 0;
				medicalFailures++;
			}
		}
	} else {
		msgLog = "The enemy is a coward";
	}
	
}

void pseudoPathfind(){
	if (!isSneaking){
		int checkX = playerEnt.currentPos.xPos;
		int checkY = playerEnt.currentPos.yPos;
		char tempMap[24][80];
		int tempHealth[24][80];
		int tempDisease[24][80];
		int tempFlee[24][80];
		int tempTarget[24][80];
		std::string tempNames[24][80];
		int tempCiv[24][80];
		char tempHand[24][80];
		int randCornerX, randCornerY;
		for (int i = 0 ; i < 80 ; i++){
			for (int j = 0 ; j < 24 ; j++){
				tempMap[j][i] = enemyMap[j][i];
				tempHealth[j][i] = enemyHealthMap[j][i];
				tempDisease[j][i] = enemyDiseaseMap[j][i];
				tempFlee[j][i] = enemyFleeingMap[j][i];
				tempNames[j][i] = enemyNamesMap[j][i];
				tempTarget[j][i] = targetingPlayerMap[j][i];
				tempCiv[j][i] = enemyCivilizationMap[j][i];
				tempHand[j][i] = enemyHandednessMap[j][i];
			}
		}
		for (int i = 0 ; i < 80 ; i++){
			for (int j = 0 ; j < 24 ; j++){
				if (enemyMap[j][i] == GOBLINCHAR || enemyMap[j][i] == KOBOLDCHAR){
					if (dungeonWalkable(i,j+1) && j < checkY-1 && whichEnemy(enemyMap[j+1][i]) == "Nothing"){
						tempMap[j][i] = ' ';
						tempMap[j+1][i] = enemyMap[j][i];
						tempHealth[j+1][i] = tempHealth[j][i];
						tempHealth[j][i] = 0;
						tempDisease[j+1][i] = tempDisease[j][i];
						tempDisease[j][i] = 0;
						tempFlee[j+1][i] = tempFlee[j][i];
						tempFlee[j][i] = 0;
						tempNames[j+1][i] = tempNames[j][i];
						tempNames[j][i] = "";
						tempTarget[j+1][i] = tempTarget[j+1][i];
						tempTarget[j][i] = 0;
						tempCiv[j+1][i] = tempCiv[j][i];
						tempCiv[j][i] = -1;
						tempHand[j+1][i] = tempHand[j][i];
						tempHand[j][i] = ' ';
					} else if (dungeonWalkable(i,j-1) && j > checkY+1 && whichEnemy(enemyMap[j-1][i]) == "Nothing"){
						tempMap[j][i] = ' ';
						tempMap[j-1][i] = enemyMap[j][i];
						tempHealth[j-1][i] = tempHealth[j][i];
						tempHealth[j][i] = 0;
						tempDisease[j-1][i] = tempDisease[j][i];
						tempDisease[j][i] = 0;
						tempFlee[j-1][i] = tempFlee[j][i];
						tempFlee[j][i] = 0;
						tempNames[j-1][i] = tempNames[j][i];
						tempNames[j][i] = "";
						tempTarget[j-1][i] = tempTarget[j][i];
						tempTarget[j][i] = 0;
						tempCiv[j-1][j] = tempCiv[j][i];
						tempCiv[j][i] = -1;
						tempHand[j-1][i] = tempHand[j][i];
						tempHand[j][i] = ' ';
					} else if (dungeonWalkable(i+1,j) && i < checkX-1 && whichEnemy(enemyMap[j][i+1]) == "Nothing"){
						tempMap[j][i] = ' ';
						tempMap[j][i+1] = enemyMap[j][i];
						tempHealth[j][i+1] = tempHealth[j][i];
						tempHealth[j][i] = 0;
						tempDisease[j][i+1] = tempDisease[j][i];
						tempDisease[j][i] = 0;
						tempFlee[j][i+1] = tempFlee[j][i];
						tempFlee[j][i] = 0;
						tempNames[j][i+1] = tempNames[j][i];
						tempNames[j][i] = "";
						tempTarget[j][i+1] = tempTarget[j][i];
						tempTarget[j][i] = 0;
						tempCiv[j][i+1] = tempCiv[j][i];
						tempCiv[j][i] = -1;
						tempHand[j][i+1] = tempHand[j][i];
						tempHand[j][i] = ' ';
					} else if (dungeonWalkable(i-1,j) && i > checkX+1 && whichEnemy(enemyMap[j][i-1]) == "Nothing"){
						tempMap[j][i] = ' ';
						tempMap[j][i-1] = enemyMap[j][i];
						tempHealth[j][i-1] = tempHealth[j][i];
						tempHealth[j][i] = 0;
						tempDisease[j][i-1] = tempDisease[j][i];
						tempDisease[j][i] = 0;
						tempFlee[j][i-1] = tempFlee[j][i];
						tempFlee[j][i] = 0;
						tempNames[j][i-1] = tempNames[j][i];
						tempNames[j][i] = "";
						tempTarget[j][i-1] = tempTarget[j][i];
						tempTarget[j][i] = 0;
						tempCiv[j][i-1] = tempCiv[j][i];
						tempCiv[j][i] = -1;
						tempHand[j][i-1] = tempHand[j][i];
						tempHand[j][i] = ' ';
					}
				}
			}
		}
		for (int i = 0 ; i < 80 ; i++){
			for (int j = 0 ; j < 24 ; j++){
				enemyMap[j][i] = tempMap[j][i];
				enemyHealthMap[j][i] = tempHealth[j][i];
				enemyDiseaseMap[j][i] = tempDisease[j][i];
				enemyFleeingMap[j][i] = tempFlee[j][i];
				enemyNamesMap[j][i] = tempNames[j][i];
				targetingPlayerMap[j][i] = tempTarget[j][i];
				enemyCivilizationMap[j][i] = tempCiv[j][i];
				enemyHandednessMap[j][i] = tempHand[j][i];
			}
		}	
	}
}

void updateEnemyHealth(){
	srand(time(0));
	int currentCiv;
	char tempTime[80];
	sprintf(tempTime, "Month %d Day %d",month, day);
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			if (enemyHealthMap[j][i] <= 0 && enemyMap[j][i] != ' '){
				enemyMap[j][i] = ' ';
				goldScore += (rand() % 10) + 1;
				enemyNamesList[enemiesSlain] = enemyNamesMap[j][i];
				enemyHealthMap[j][i] = -1;
				currentCiv = enemyCivilizationMap[j][i];
				if (enemyNamesList[enemiesSlain] == leaders[currentCiv]){
					//this handles killing civilization leaders
					msgLog = "You killed a great leader!";
					worldHistory = worldHistory+"On "+tempTime+", The great leader of "+civilizations[currentCiv]+" ("+leaders[currentCiv]+") was killed by "+playerName+"\n";
					leaders[currentCiv] = returnGoblinName(3);
					worldHistory = worldHistory+"On "+tempTime+", "+leaders[currentCiv]+" the Goblin rose to power in "+civilizations[currentCiv]+"\n";
				} else {
					worldHistory = worldHistory+"On "+tempTime+", "+playerName+" struck down "+enemyNamesMap[j][i]+"\n";
					msgLog = "The enemy died!";
				}
				enemiesSlain++;
				enemyNamesMap[j][i] = "";
				targetingPlayerMap[j][i] = 0;
				enemyCivilizationMap[j][i] = -1;
			}
			if (enemyHealthMap[j][i] > 0 && enemyMap[j][i] != ' '){
				enemyFleeingMap[j][i] = shouldFlee(i,j);
				targetingPlayerMap[j][i] = !shouldFlee(i,j);
			}
		}
	}
}

int isEnemyAt(int xPos, int yPos){
	if (whichEnemy(enemyMap[yPos][xPos]) != "Nothing"){
		return 1;
	} else {
		return 0;	
	}
}
