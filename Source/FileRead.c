#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Player.h"
#include "DrawScreen.h"
#include "Enemies.h"
#include "MapGenerator.h"
#include <ncurses.h>

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
