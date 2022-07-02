#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include "History.h"
#include "Enemies.h"
#include "Main.h"

std::string civilizations[4];
int civilizationPeaceful[4];
int civilizationRacist[4];
int civilizationSmart[4];

std::string returnGoblinName(int pairs){
	std::string tempString = "";
	int consonantValue;
	int vowelValue;
	char syllablePair[2];
	for (int i = 0 ; i < pairs ; i++){
		srand(time(0)+pairs+i);
		consonantValue = rand() % 8;
		vowelValue = rand() % 3;
		if (i == 0){
			syllablePair[0] = goblinConsonants[consonantValue] - 32;
		} else {
			syllablePair[0] = goblinConsonants[consonantValue];
		}
		syllablePair[1] = goblinVowels[vowelValue];
		tempString = tempString+syllablePair;
		if (rand () % 4 == 0){
			if (rand() % 2 == 0){
				tempString = tempString+"j";
			} else {
				tempString = tempString+"r";
			}
		}
	}
	return tempString;
}

void generateCivilizations(){
	srand(seedMain);
	for (int i = 0 ; i < 4 ; i++){
		civilizations[i] = returnGoblinName((rand() % 6)+2);
		civilizationPeaceful[i] = rand() % 4 != 0;
		civilizationRacist[i] = rand() % 2;
		civilizationSmart[i] = rand() % 2;
	}
}

