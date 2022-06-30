#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include "Player.h"

int enemiesSlain;
int goldFound;
int bloodLost;
int foodEaten;
int gluttonCount;
int potionsQuaffed;
int itemsFound;
int itemsLost;
int medicalSuccesses;
int medicalFailures;

std::string generateLegacy(){
	std::string tempString = "";
	
	if (foodEaten < gluttonCount){
		tempString = tempString+"You were known as a glutton.\n";
	}

	if (foodEaten < potionsQuaffed){
		tempString = tempString+"You were known as a potioholic.\n";
	}

	if (goldFound <= 250){
		tempString = tempString+"You died pennyless.\n";
	} else if (goldFound > 250 && goldFound <= 500){
		tempString = tempString+"You died with a fair amount of money to your name. \n";
	} else if (goldFound > 500){
		tempString = tempString+"You were famous for your riches.\n";
	}

	if (bloodLost > 500 && bloodLost <= 1000){
		tempString = tempString+"You had an incredible natural recovery rate.\n";
	} else if (bloodLost > 1000){
		tempString = tempString+"You had a recovery rate like that of a wolverine.\n";
	}

	if (enemiesSlain <= 10){
		tempString = tempString+"You were helpless against the weakest enemy.\n";
	} else if (enemiesSlain > 10 && enemiesSlain <= 100){
		tempString = tempString+"You killed a few enemies.\n";
	} else if (enemiesSlain > 100){
		tempString = tempString+"You were renowned for slaying hundreds of monsters.\n";
	}

	if (itemsLost < itemsFound){
		tempString = tempString+"You were famous for hoarding items.\n";
	} else {
		tempString = tempString+"You lived a minimal life.\n";
	}

	if (medicalFailures < medicalSuccesses){
		tempString = tempString+"You were very medically inclined.\n";
	} else if (medicalFailures == medicalSuccesses){
		tempString = tempString+"You were of neutral health.\n";
	} else {
		tempString = tempString+"You were quite unhealthy.\n";
	}

	return tempString;
}

void saveLegacy(){
	char tempString[4096];
	FILE *ptr;
	ptr = fopen("../Save/History.txt", "a");
	if (NULL==ptr){
		killPlayer("Could not open history file");
	}
	sprintf(tempString, "\n ## %s ## \n", playerName.c_str());
	fputs(tempString, ptr);
	
	sprintf(tempString, "%s",generateLegacy().c_str());
	fputs(tempString, ptr);

	fclose(ptr);
}
