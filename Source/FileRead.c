#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "Player.h"
#include "DrawScreen.h"
#include "Enemies.h"
#include "MapGenerator.h"
#include <ncurses.h>
#include "Movement.h"
#include "Main.h"
#include "Inventory.h"
#include "PlayerCreator.h"
#include "Legacy.h"

void initFileRead(){
	FILE *ptr;
	char str[50];
	ptr = fopen("../Init/init.txt", "r");

	if (NULL==ptr){
		killPlayer("Unable to read init file\n");
	}
	int index = 0;
	while (fgets(str, 50, ptr) != NULL){
		switch(index){
			case 0:
				if (str[0] == 'Y'){
					grossStuff = 1;
				} else {
					grossStuff = 0;
				}	
				break;
			case 1:
				if (str[0] == 'Y'){
					noFov = 1;
				} else {
					noFov = 0;
				}
				break;
			case 2:
				if (str[0] == 'Y'){
					inDebugMode = 1;
				} else {
					inDebugMode = 0;
				}
				break;
			case 3:
				if (str[0] == 'Y'){
					disabledCreator = 1;
				} else {
					disabledCreator = 0;
				}
				break;
			case 4:
				defaultName = str;
				break;
			case 5:
				defaultSex = str[0];
				break;
			case 6:
				defaultRace = str;
				break;
			default:
				continue;
		}
		index++;
	}

	fclose(ptr);
}

void worldFileRead(){
	FILE *ptr;
	char str[50];
	ptr = fopen("../Init/world_parameters.txt","r");

	if (NULL==ptr){
		killPlayer("Unable to read world parameter file\n");
	}
	int index = 0;
	while(fgets(str,50,ptr) != NULL){
		switch(index){
			case 0:
				spawnRate = atoi(str);
				break;
			case 1:
				baseEnemyHealth = atoi(str);
				break;
			case 2:
				baseEnemyDamage = atoi(str);
				break;
			case 3:
				diseaseRate = atoi(str);
				break;
			case 4:
				diseaseSpreadRate = atoi(str);
				break;
			case 5:
			      	treeSpawnRate = atoi(str);
				break;	
			case 6:
				dungeonSpawnRate = atoi(str);
				break;
			case 7:
				if (str[0] == 'Y'){
					canFireSpread = 1;
				} else {
					canFireSpread = 0;
				}
				break;
			case 8:
				bloodTrailThreshold = atoi(str);
				break;
			case 9:
				DEADLYBLOODLOSS = atoi(str);
				break;
		}
		index++;
	}
}

void savePlayerData(){
	/**	Format of save file:
	 *	Global seed
	 *	Player xPos
	 *	Player yPos
	 *	Player inventory
	 *	Player sex
	 *	Player name
	 *	Player star sign
	 *	Player stats
	 *	Player gold
	 *	Player health
	 *	Player weight
	 *	Player nourishment
	 *	Player hydration
	 *	Player temperature
	 *	Infection/Disease data
	 *	Current biome
	 *	Current map
	 */
	char tempString[80];
	FILE *ptr;
	ptr = fopen("../Save/PlayerData.txt", "w");
	if (NULL==ptr){
		killPlayer("Could not open save file");
	}

	fflush(ptr);

	//save global seed
	sprintf(tempString, "Global seed: %d\n",seedMain);
	fputs(tempString,ptr);
	
	
	fputs("\n## Player Data ##\n", ptr);

	//save player position
	sprintf(tempString, "xPos: %d\n",playerEnt.currentPos.xPos);
	fputs(tempString,ptr);
	sprintf(tempString, "yPos %d\n",playerEnt.currentPos.yPos);
	fputs(tempString,ptr);

	//save player inventory
	fputs("Inventory\n",ptr);
	for (int i = 0 ; i < 16 ; i++){
		sprintf(tempString, " - %s\n",getItem(i));
		fputs(tempString,ptr);
	}

	//save player sex
	sprintf(tempString, "Sex: %c\n", playerEnt.currentSex.sex);
	fputs(tempString, ptr);

	//save player name
	sprintf(tempString, "Name: %s\n", playerName.c_str());
	fputs(tempString,ptr);

	sprintf(tempString, "Race: %s\n", playerRace.c_str());
	fputs(tempString, ptr);

	//save player star sign
	sprintf(tempString, "Star sign: %c\n", playerEnt.currentSign.sign);
	fputs(tempString, ptr);

	//save player stats
	sprintf(tempString, "Strength: %d\n", playerEnt.currentStats.strength);
	fputs(tempString, ptr);
	sprintf(tempString, "Agility: %d\n", playerEnt.currentStats.agility);
	fputs(tempString, ptr);
	sprintf(tempString, "Intelligence: %d\n", playerEnt.currentStats.intelligence);
	fputs(tempString, ptr);
	sprintf(tempString, "Handedness: %s\n", playerHandedness.c_str());
	fputs(tempString, ptr);

	//save gold count
	sprintf(tempString, "Gold: %d\n",goldScore);
	fputs(tempString,ptr);

	//save player health
	/**
	 *	Head
	 *	Torso
	 *	Left Arm
	 *	Right Arm
	 *	Left Leg
	 *	Right Leg
	 *	Skin
	 *	Blood Level
	 *	Blood Loss Rate
	 */
	sprintf(tempString, "Head: %d\n", playerEnt.head.bpHP.currentHealth);
	fputs(tempString, ptr);
	sprintf(tempString, "Torso: %d\n", playerEnt.torso.bpHP.currentHealth);
	fputs(tempString, ptr);
	sprintf(tempString, "Left Arm: %d\n", playerEnt.leftArm.bpHP.currentHealth);
	fputs(tempString, ptr);
	sprintf(tempString, "Right Arm: %d\n", playerEnt.rightArm.bpHP.currentHealth);
	fputs(tempString, ptr);
	sprintf(tempString, "Left Leg: %d\n", playerEnt.leftLeg.bpHP.currentHealth);
	fputs(tempString, ptr);
	sprintf(tempString, "Right Leg: %d\n", playerEnt.rightLeg.bpHP.currentHealth);
	fputs(tempString, ptr);
	sprintf(tempString, "Skin: %d\n", playerEnt.skin.bpHP.currentHealth);
	fputs(tempString, ptr);

	sprintf(tempString, "Blood Level: %d\n",bloodCount);
	fputs(tempString, ptr);
	sprintf(tempString, "Blood Loss Rate: %d\n",bloodLossRate);
	fputs(tempString, ptr);
	
	//save weight
	sprintf(tempString, "Weight(LBS): %d\n",playerEnt.currentWeight.pounds);
	fputs(tempString, ptr);

	//save nourishment
	sprintf(tempString, "Nourishment: %d\n",foodScore);
	fputs(tempString, ptr);

	//save hydration
	sprintf(tempString, "Hydration: %d\n",playerEnt.currentHydration.hydration);
	fputs(tempString, ptr);

	//save temperature
	sprintf(tempString, "Temperature(C): %d\n",playerEnt.currentTemperature.celsius);
	fputs(tempString, ptr);

	//save infection status
	fputs("\n## Disease/Infection Data ##\n", ptr);
	sprintf(tempString, "isInfected: %d\n",isInfected);
	fputs(tempString, ptr);
	sprintf(tempString, "isDiseased: %d\n",isDiseased);
	fputs(tempString, ptr);
	sprintf(tempString, "isCrying: %d\n",isCrying);
	fputs(tempString, ptr);
	sprintf(tempString, "isCongested: %d\n",isCongested);
	fputs(tempString, ptr);
	sprintf(tempString, "hasRash: %d\n",hasRash);
	fputs(tempString, ptr);
	sprintf(tempString, "hasDiarrhea: %d\n",hasDiarrhea);
	fputs(tempString, ptr);

	//save world data
	fputs("\n## World Data ##\n", ptr);
	sprintf(tempString, "Biome: %c\n", biome);
	fputs(tempString, ptr);
	fputs("End Screen:\n", ptr);
	for (int j = 0 ; j < 24 ; j++){
		for (int i = 0 ; i < 80 ; i++){
			fputc(map[j][i], ptr);
		}
		fputc('\n', ptr);	
	}

	//save legacy
	fputs("\n## Legacy ##\n", ptr);
	fputs(generateLegacy().c_str(), ptr);
	
	fputs("\n## Enemies Slain ## \n", ptr);
	for (int i = 0 ; i < 512 ; i++){
		if (enemyNamesList[i] != ""){
			sprintf(tempString," - %s\n",enemyNamesList[i].c_str());
			fputs(tempString, ptr);
			continue;
		}
	}
	fclose(ptr);

	saveLegacy();
}
