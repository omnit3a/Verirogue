#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "Player.h"
#include "MapGenerator.h"

int foodCount = 0;

int foodScore = 100;

int goldScore = 0;

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

	playerEnt.currentWeight.pounds = 175;
	playerEnt.currentFlammability.flammabilityLevel = 50;
	playerEnt.currentTemperature.celsius = 37;
	playerEnt.currentTemperature.fahrenheit = 98;
	playerEnt.currentHydration.hydration = 60;

	playerEnt.head.bpHP.currentHealth = 100;
	playerEnt.torso.bpHP.currentHealth = 100;
	playerEnt.leftArm.bpHP.currentHealth = 100;
	playerEnt.rightArm.bpHP.currentHealth = 100;
	playerEnt.leftLeg.bpHP.currentHealth = 100;
	playerEnt.rightLeg.bpHP.currentHealth = 100;

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
