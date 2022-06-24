#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <ncurses.h>
#include "Player.h"
#include "MapGenerator.h"
#include "DrawScreen.h"
#include "Movement.h"
#include "DrawUI.h"

int bloodCount = 511;		//this is the amount of blood in the human body in litres * 10i0

int bloodLossRate = 0;

int foodCount = 0;

int foodScore = 100;

int goldScore = 0;

int isInfected, hasDiarrhea, isCongested, hasRash, infectionStart, isDiseased = 0;

/**
 *	10 = Fists
 *	15 = Sword
 *	20 = Axe
 */
int currentWeapon = 10;

Humanoid playerEnt;

void setupPlayer(int x, int y, int entID){
	playerEnt.entityID = entID;
	playerEnt.currentPos.xPos = x;
	playerEnt.currentPos.yPos = y;

	if (playerEnt.currentSex.sex == 'M' || playerEnt.currentSex.sex == 'm'){
		playerEnt.currentSex.sex = 'M';
	} else if (playerEnt.currentSex.sex == 'F' || playerEnt.currentSex.sex == 'f'){
		playerEnt.currentSex.sex = 'F';
	} else if (playerEnt.currentSex.sex == 'U' || playerEnt.currentSex.sex == 'u'){
		playerEnt.currentSex.sex = 'U';
	}

	switch (playerEnt.currentSign.sign){
		case 'A':
			srand(time(0)+10);
			playerEnt.currentStats.strength = (rand() % 25)+75;
			playerEnt.currentStats.agility = (rand() % 25)+25;
			playerEnt.currentStats.intelligence = 25;
			break;
		case 'B':
			srand(time(0)+20);
			playerEnt.currentStats.strength = (rand() % 25)+25;
			playerEnt.currentStats.agility = (rand() % 25)+75;
			playerEnt.currentStats.intelligence = 65;
			break;
		case 'C':
			srand(time(0)+30);
			playerEnt.currentStats.strength = 25;
			playerEnt.currentStats.agility = (rand() % 10)+40;
			playerEnt.currentStats.intelligence = (rand() % 25)+75;
			break;
	}

	playerEnt.currentWeight.pounds = (playerEnt.currentStats.strength*3) - lround(playerEnt.currentStats.agility/3);
	if (playerEnt.currentWeight.pounds < 100){
		playerEnt.currentWeight.pounds += 100;
	}
	playerEnt.currentFlammability.flammabilityLevel = 50;
	playerEnt.currentTemperature.celsius = 37;
	playerEnt.currentTemperature.fahrenheit = 98;
	playerEnt.currentHydration.hydration = 60;

	playerEnt.skin.bpHP.currentHealth = 100;
	playerEnt.head.bpHP.currentHealth = 100;
	playerEnt.torso.bpHP.currentHealth = 100;
	playerEnt.leftArm.bpHP.currentHealth = 100;
	playerEnt.rightArm.bpHP.currentHealth = 100;
	playerEnt.leftLeg.bpHP.currentHealth = 100;
	playerEnt.rightLeg.bpHP.currentHealth = 100;

	playerEnt.skin.currentPos.xPos = x;
	playerEnt.skin.currentPos.yPos = y;
	playerEnt.head.currentPos.xPos = x;
	playerEnt.head.currentPos.yPos = y;
	playerEnt.torso.currentPos.xPos = x;
	playerEnt.torso.currentPos.yPos = y;
	playerEnt.leftArm.currentPos.xPos = x;
	playerEnt.leftArm.currentPos.yPos = y;
	playerEnt.rightArm.currentPos.xPos = x;
	playerEnt.rightArm.currentPos.yPos = y;
	playerEnt.leftLeg.currentPos.xPos = x;
	playerEnt.leftLeg.currentPos.yPos = y;
	playerEnt.rightLeg.currentPos.xPos = x;
	playerEnt.rightLeg.currentPos.yPos = y;

	map[y][x] = '<';
}

void killCheck(){
	if (playerEnt.head.bpHP.currentHealth <= 0){
		clear();
		endScreen();
		printf("You died of major head trauma! Be more careful next time.\n");
		exit(0);
	}
	if (playerEnt.torso.bpHP.currentHealth <= 0){
		clear();
		endScreen();
		printf("You died of internal bleeding! Be more careful next time.\n");
		exit(0);
	}
	if (playerEnt.skin.bpHP.currentHealth <= 0){
		clear();
		endScreen();
		printf("You were skinned alive! Be more careful next time.\n");
		exit(0);
	}
	if (bloodCount < DEADLYBLOODLOSS){
		clear();
		endScreen();
		printf("You died of external bleeding! Be more careful next time.\n");
		exit(0);
	}
	
}

void bleedCheck(){
	bloodLossRate = 0;
	if (playerEnt.head.bpHP.currentHealth < 25){
		bloodLossRate += 10;
	}
	if (playerEnt.torso.bpHP.currentHealth < 15){
		bloodLossRate += 10;
	}
	if (playerEnt.leftArm.bpHP.currentHealth < 25){
		bloodLossRate += 10;
	}
	if (playerEnt.rightArm.bpHP.currentHealth < 25){
		bloodLossRate += 10;
	}
	if (playerEnt.skin.bpHP.currentHealth < 20){
		bloodLossRate += 20;
	}
	bloodCount -= bloodLossRate/10;
}

void infectionCheck(){
	if (playerEnt.skin.bpHP.currentHealth < 50 && isInfected == 0){
		msgLog = "You are develop an infection!";
		isInfected = 1;
		infectionStart = turn;
		srand(time(0));	
	
		/**
		*	0 = Common Cold			most common viral infection
	 	*	1 = Impetigo			one of the most common bacterial infections
		*	2 = Giardia intestinalis	one of the most common parasitic infections
	 	*	
	 	*/
		int infectionType = rand() % 3;
		switch (infectionType){
			case 0:
				isCongested = 1;
				break;
			case 1:
				hasRash = 1;
				break;
			case 2:
				hasDiarrhea = 1;
				break;	
		}
	}
	if (turn-infectionStart > 500){
		isInfected = 0;
		isCongested = 0;
		hasRash = 0;
		hasDiarrhea = 0;
	}
}
