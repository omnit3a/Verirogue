#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>

int enemiesSlain;
int goldFound;
int bloodLost;
int foodEaten;
int gluttonCount;
int potionsQuaffed;

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
	if (bloodLost > 500){
		tempString = tempString+"You had incredible natural recovery.\n";
	}
	return tempString;
}
