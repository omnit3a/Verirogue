#include <stdio.h>
#include <stdlib.h>
#include "MapGenerator.h"
#include "NPC.h"

char npcMap[24][80];
int npcCount = 0;
int npcPos[MAX_NPCS][2];

void placeNPCS(int amount){
	int count = 0;
	int xPos, yPos;
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			npcMap[j][i] = ' ';
		}
	}
	npcCount = 0;
	if (amount < MAX_NPCS){
		while (count < amount){
			xPos = (rand() % 79) + 1;
			yPos = (rand() % 23) + 1;
			if (map[yPos][xPos] != '#' && npcMap[yPos][xPos] == ' '){
				npcMap[yPos][xPos] = '@';
								
				npcPos[count][0] = yPos;	//save x and y pos
				npcPos[count][1] = xPos;

				count++;
				npcCount++;
				continue;
			} else {
				continue;
			}
		}
	}
}

void pathfindNPCS(){
	int tempCount = 0;
	int checkX;
	int checkY;
	int tempDir;
	for (int i = 0 ; i < npcCount ; i++){		//instead of checking every tile on the map, only check tiles that contain npcs
		checkX = npcPos[i][1];
		checkY = npcPos[i][0];	
		if (npcMap[checkY][checkX] != ' '){
			tempDir = rand() % 4;
			switch (tempDir){
				case 0:
					if (npcMap[checkY-1][checkX] == ' ' && map[checkY-1][checkX] != '#' && checkY > 1){
						npcMap[checkY-1][checkX] = npcMap[checkY][checkX];
						npcMap[checkY][checkX] = ' ';
						npcPos[i][0]--;
					}
					break;
				case 1:
					if (npcMap[checkY+1][checkX] == ' ' && map[checkY+1][checkX] != '#' && checkY < 23){
						npcMap[checkY+1][checkX] = npcMap[checkY][checkX];
						npcMap[checkY][checkX] = ' ';
						npcPos[i][0]++;
					}
					break;
				case 2:
					if (npcMap[checkY][checkX-1] == ' ' && map[checkY][checkX-1] != '#' && checkX > 1){
						npcMap[checkY][checkX-1] = npcMap[checkY][checkX];
						npcMap[checkY][checkX] = ' ';
						npcPos[i][1]--;
					}
					break;
				case 3:
					if (npcMap[checkY][checkX+1] == ' ' && map[checkY][checkX+1] != '#' && checkX < 80){
						npcMap[checkY][checkX+1] = npcMap[checkY][checkX];
						npcMap[checkY][checkX] = ' ';
						npcPos[i][1]++;
					}
					break;
			}
		}
	}
}
