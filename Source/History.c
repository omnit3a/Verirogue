#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <time.h>
#include "History.h"
#include "Enemies.h"
#include "Main.h"

std::string civilizations[4];
std::string leaders[4];
int civilizationStyle[4];
int civilizationPeaceful[4];
int civilizationRacist[4];
int civilizationSmart[4];

std::string worldHistory;

std::string returnGoblinName(int pairs){
	std::string tempString = "";
	int consonantValue;
	int vowelValue;
	char syllablePair[2];
	for (int i = 0 ; i < pairs ; i++){
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
		srand(seedMain+i);
		civilizations[i] = returnGoblinName((rand() % 6)+2);
		civilizationStyle[i] = rand() % 6;
		civilizationPeaceful[i] = rand() % 4 == 0;
		civilizationRacist[i] = rand() % 2 == 0;
		civilizationSmart[i] = rand() % 4 == 0;
		srand(seedMain+(i*2));
		leaders[i] = returnGoblinName(3);
	}
}

std::string generateCivData(){
	std::string tempString;
	tempString = tempString+"## Civilizations ##\n";
	for (int i = 0 ; i < 4 ; i++){
		tempString = tempString+" - The goblin civilization "+civilizations[i]+"\n";
		tempString = tempString+"   - Leader: "+leaders[i]+"\n";
		if (civilizationPeaceful[i]){
			tempString = tempString+"   - They are a peaceful civilization.\n";
		} else {
			tempString = tempString+"   - They are a engaging in constant war.\n";
		}
		if (civilizationRacist[i]){
			tempString = tempString+"   - They have a deep hatred for the kobold race.\n";
		}
		if (civilizationSmart[i]){
			tempString = tempString+"   - They are exceptionally advanced.\n";
		}
		switch (civilizationStyle[i]){
			case 0:
				tempString = tempString+"   - They have a gaudy style.\n";
				break;
			case 1:
				tempString = tempString+"   - They have an eclectic style.\n";
				break;
			case 2:
				tempString = tempString+"   - They have a monolithic style.\n";
				break;
			case 3:
				tempString = tempString+"   - They have an oppresive style.\n";
				break;
			case 4:
				tempString = tempString+"   - They have a revivalist style.\n";
				break;
			case 5:
				tempString = tempString+"   - They have a classical style.\n";
				break;
		}	
		tempString = tempString+"\n";
	}
	return tempString;
}

