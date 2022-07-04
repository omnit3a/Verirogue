#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "DrawScreen.h"
#include "MapGenerator.h"
#include "FastNoiseLite.h"
#include "Player.h"
#include "Movement.h"
#include "Main.h"
#include "Enemies.h"
#include "Inventory.h"
#include "Gas.h"
#include "DrawUI.h"

FastNoiseLite noise;

int dungeonHasFire = 0;

int coastmap[24][80];

void generateCoastmap(){
	noise.SetFrequency(0.05);
	noise.SetSeed(seedMain);
	noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
	noise.SetFractalType(FastNoiseLite::FractalType_None);
	noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_EuclideanSq);
	int value;
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			coastmap[j][i] = 0;
			value = abs(lround((noise.GetNoise((float) playerEnt.currentPos.xPos+i, (float) playerEnt.currentPos.yPos+j)+0.40)/0.10));
			if (value <= 1){
				coastmap[j][i] = 1;
			} else {
				coastmap[j][i] = 0;
			}
		}
	}
}

int returnCoastmapAt(int x, int y){
	return coastmap[y][x];
}

char map[24][80];

char surroundingChar[4];

char underPlayer;

int treeSpawnRate = 100;
int dungeonSpawnRate = 50;
int canFireSpread = 1;

void generateMap(){
	generateCoastmap();
	noise.SetFrequency(0.04);
	noise.SetSeed(seedMain);
	noise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
	noise.SetFractalType(FastNoiseLite::FractalType_Ridged);
	int value;
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			value = abs(lround((noise.GetNoise((float) playerEnt.currentPos.xPos+i, (float) playerEnt.currentPos.yPos+j)+0.5)/0.09));
			if (playerEnt.currentPos.xPos+i > 136 || playerEnt.currentPos.xPos+i < -56 || playerEnt.currentPos.yPos+j > 60 || playerEnt.currentPos.yPos+j < -36){
				srand(seedFromPosition(playerEnt.currentPos.xPos+i, playerEnt.currentPos.yPos+j));
				if (rand() % 10 == 0){
					map[j][i] = '*';
				} else {
					map[j][i] = ' ';
				}
				continue;
			}
			if (returnCoastmapAt(i,j) == 0 && value > 3){
				if (value >= 11 && value <= 13){
					map[j][i] = '^';
				} else if (value > 13){
					map[j][i] = 'A';
				} else {
					map[j][i] = '.';
				}
			} else {
				map[j][i] = '~';
			}
			srand(seedFromPosition(i+playerEnt.currentPos.xPos,j+playerEnt.currentPos.yPos));
			if (rand() % dungeonSpawnRate == 0 && map[j][i] == 'A'){
				map[j][i] = '>';
			}
			if (rand() % 100 == 0 && map[j][i] == '.'){
				map[j][i] = 'H';
			}
		}
	}
	surroundingChar[0] = map[11][40];
	surroundingChar[1] = map[12][41];
	surroundingChar[2] = map[13][40];
	surroundingChar[3] = map[12][39];

	//this is the character underneath the player
	underPlayer = map[12][40];
}

char returnHeightmapAt(int x, int y){
	return map[y][x];
}

char returnCountryAt(int x, int y){
	noise.SetFrequency(0.1);
	noise.SetSeed(seedMain);
	noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
	noise.SetCellularJitter(2);
	noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Manhattan);
	int value = abs(lround((noise.GetNoise((float) playerEnt.currentPos.xPos+x, (float) playerEnt.currentPos.yPos+y)+0.2)/0.2));
	if (value < 1){
		return '#';
	} else {
		return ' ';
	}
}

int startX, startY;

char roomF = '.';
char hallway = '+';

void generateEngraving(int x, int y){
	if ((map[y-1][x] != ' ' || map[y+1][x] != ' ' || map[y][x-1] != ' ' || map[y][x+1] != ' ')&&map[y][x] == '#'){
		map[y][x] = (rand() % 26)+65;
	}
}

void generateWall(){
	char tempMap[24][80];
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			if (((map[j-1][i] != ' ' || map[j+1][i] != ' ' || map[j][i-1] != ' ' || map[j][i+1] != ' ' || map[j-1][i-1] != ' ' || map[j-1][i+1] != ' ' || map[j+1][i-1] != ' ' || map[j+1][i+1] != ' ') && map[j][i] == ' ')||(j==0 || i==0 || j==23 || i==79)){
				tempMap[j][i] = '#';
			} else {
				tempMap[j][i] = map[j][i];
			}
		}
	}
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			map[j][i] = tempMap[j][i];
		}
	}
}

void generateDungeon(int maxWidth, int maxHeight){
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			map[j][i] = ' ';
			seenMap[j][i] = ' ';
		}
	}
	srand(time(0));
	int xPos, yPos, width, height, rooms;
	rooms = 9;
	int roomCenters[rooms][2];
	for (int i = 0 ; i < rooms ; i++){
		xPos = rand() % (80-(maxWidth-5))+4;
		yPos = rand() % (24-(maxHeight-5))+4;
		width = (rand() % maxWidth)+3;
		height = (rand() % maxHeight)+3;
		
		for (int n = yPos ; n < yPos+height ; n++){
			for (int m = xPos ; m < xPos+width ; m++){
				map[n][m] = roomF; 
			}
		}
		roomCenters[i][0] = xPos+(width/2);
		roomCenters[i][1] = yPos+(height/2);	
	}
	int connectedRooms[rooms*3][2];
	int roomA, roomB, xs, ys, xe, ye;
	int retries;
	for (int i = 1 ; i < 33 ; i++){
		retries = 0;
		getRoom:
			if (retries == 64){
				break;
			}
			roomA = rand() % rooms;
			roomB = rand() % rooms;
			if (roomA == roomB){
				goto getRoom;
			}
			if (connectedRooms[i-1][0] == roomA && connectedRooms[i-1][1] == roomB){
				retries++;
				goto getRoom;
			}	
		xs = roomCenters[roomA][0];
		ys = roomCenters[roomA][1];
		xe = roomCenters[roomB][0];
		ye = roomCenters[roomB][1];
		while (xs != xe){
			if (xs <= xe){
				xs++;
				if (map[ys][xs] == roomF){
					continue;
				}
				map[ys][xs] == hallway;
			} else if (xs > xe){
				xs--;
				if (map[ys][xs] == roomF){
					continue;
				}
				map[ys][xs] = hallway;	
			}
		}
		while (ys != ye){
			if (ys <= ye){
				ys++;
				if (map[ys][xs] == roomF){
					continue;
				}
				map[ys][xs] = hallway;
			} else if (ys > ye){
				ys--;
				if (map[ys][xs] == roomF){
					continue;
				}
				map[ys][xs] = hallway;
			}
		}
		connectedRooms[i][0] = roomA;
		connectedRooms[i][1] = roomB;
	}
	end:
		startX = (rand() % 78) + 1;
		startY = (rand() % 22) + 1;
		if (map[startY][startX] == ' '){
			goto end;
		} else {
			map[startY][startX] = '<';
		}
		srand(time(0));
		int roomXGold, engravingX;
		int roomYGold, engravingY;
		for (int i = 0 ; i < 16 ; i++){
			srand(time(0)+i);
			roomXGold = rand() % 80;
			roomYGold = rand() % 24;
			if (dungeonWalkable(roomXGold, roomYGold) && map[roomYGold][roomXGold] == '.'){
				map[roomYGold][roomXGold] = '$';
			}
			continue;
		}
		generateWall();
		for (int i = 0 ; i < 64 ; i++){
			srand(time(0)+i-1);
			engravingX = rand() % 80;
			engravingY = rand() % 24;
		       	if (map[engravingY][engravingX]	!= ' '){
				generateEngraving(engravingX, engravingY);
			}
			continue;
		}
		placeEnemies();
		placeItems();
		
}

char returnDungeonmapAt(int x, int y){
	return map[y][x];
}


/**
 *	zoomed in stuff starts here...
 *
 */
void generateCloseField(){
	msgLog = "You explore the field";
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			if (rand() % 8 == 0){
				map[j][i] = '*';
			} else {
				map[j][i] = '.';
			}
		}
	}
}

void generateCloseHill(){
	msgLog = "You explore the forest";
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			if (rand() % 2 == 0){
				map[j][i] = '^';
			} else {
				map[j][i] = '.';
			}
		}
	}
	int treeCount;
	int grassCount;
	char tempHills[24][80];
	int adjEmpty;
	for (int r = 0 ; r < 8 ; r++){
		for (int x = 0 ; x < 80 ; x++){
			for (int y = 0 ; y < 24 ; y++){
				treeCount = 0;
				grassCount = 0;
				if (map[y-1][x] == '^'){
					treeCount++;
				} else {
					grassCount++;
				}
				if (map[y+1][x] == '^'){
					treeCount++;
				} else {
					grassCount++;
				}
				if (map[y][x-1] == '^'){
					treeCount++;
				} else {
					grassCount++;
				}
				if (map[y][x+1] == '^'){
					treeCount++;
				} else {
					grassCount++;
				}
				if (grassCount <= treeCount){
					tempHills[y][x] = '.';
				} else {
					tempHills[y][x] = '^';
				}
				if (tempHills[y-1][x] == '.' && tempHills[y+1][x] == '.' && tempHills[y][x-1] == '.' && tempHills[y][x+1] == '.'){
					tempHills[y][x] = '.';
				}	
			}
		}
	}
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			if (rand() % 16 == 0 && tempHills[j][i] == '.'){
				map[j][i] = '*';
			} else {
				map[j][i] = tempHills[j][i];
			}
		}
	}
}

void generateCloseMountain(){
	msgLog = "You explore the mountains";
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			if (rand() % 3 == 0){
				map[j][i] = 'A';
			} else {
				map[j][i] = '.';
			}
		}
	}
	int mountCount;
	int grassCount;
	char tempMountain[24][80];
	int adjEmpty;
	for (int r = 0 ; r < 8 ; r++){
		for (int x = 0 ; x < 80 ; x++){
			for (int y = 0 ; y < 24 ; y++){
				mountCount = 0;
				grassCount = 0;
				if (map[y-1][x] == 'A'){
					mountCount++;
				} else {
					grassCount++;
				}
				if (map[y+1][x] == 'A'){
					mountCount++;
				} else {
					grassCount++;
				}
				if (map[y][x-1] == 'A'){
					mountCount++;
				} else {
					grassCount++;
				}
				if (map[y][x+1] == 'A'){
					mountCount++;
				} else {
					grassCount++;
				}
				if (grassCount <= mountCount){
					tempMountain[y][x] = 'A';
				} else {
					tempMountain[y][x] = '.';
				}
				if (tempMountain[y-1][x] == 'A' && tempMountain[y+1][x] == 'A' && tempMountain[y][x-1] == 'A' && tempMountain[y][x+1] == 'A'){
					tempMountain[y][x] = 'A';
				}	
			}
		}
	}
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			map[j][i] = tempMountain[j][i];
		}
	}
}

void generateCloseUp(){
	switch (biome){
		case 'f':
			generateCloseField();
			break;
		case 'h':
			generateCloseHill();
			break;
		case 'm':
			generateCloseMountain();
			break;
	}
	switch (underPlayer){
		case '.':
			biome = 'f';
			generateCloseField();
			break;
		case '^':
			biome = 'h';
			generateCloseHill();
			break;
		case 'A':
			biome = 'm';
			generateCloseMountain();
			break;
	}
	
	surroundingChar[0] = map[11][40];
	surroundingChar[1] = map[12][41];
	surroundingChar[2] = map[13][40];
	surroundingChar[3] = map[12][39];

	//this is the character underneath the player
	underPlayer = map[12][40];
}

/**
 *	...and ends here
 */

int townMap[24][80];

int townNPCS[9][4];

/**
 *	Code to generate town. I might replace this with Wave function collapse.
 *	For now though, I'm gonna use a technique similar to the dungeon generator
 */
void generateTown(int maxWidth, int maxHeight, int amountOfBuildings){
	biome = 't';
	msgLog = "You enter the village";
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			map[j][i] = '.';
		}
	}
	int width, height, xPos, yPos, doorX, doorY;
	char floorChar = '+';
	char wallChar = '#';
	int doorWallPos;
	for (int r = 0 ; r < amountOfBuildings ; r++){
		xPos = (rand() % (79-(maxWidth+5)))+2;
		yPos = (rand() % (23-(maxHeight+5)))+2;
		width = (rand() % maxWidth)+5;
		height = (rand() % maxHeight)+5;
		doorWallPos = rand() % 4;
		if (map[yPos-1][xPos-1] != '.' || map[yPos+height+1][xPos+width+1] != '.' || map[yPos-1][xPos+width+1] != '.' || map[yPos+height+1][xPos-1] != '.'){ //prevent overlapping houses
			continue;
		}
		if (map[yPos-1][lround(xPos+(width/2))] != '.' || map[yPos+height+1][lround(xPos+(width/2))] != '.' || map[lround(yPos+(height/2))][xPos-1] != '.' || map[lround(yPos+(height/2))][xPos+width+1] != '.'){	//makes sure houses don't overlap (again)
			continue;
		}
		for (int i = xPos-1 ; i < xPos+width+1 ; i++){							//generate gap around houses
			for (int j = yPos-1 ; j < yPos+height+1 ; j++){
				if (i == xPos-1 || j == yPos-1 || i == xPos+width+1 || j == yPos+height+1){
					map[j][i] = 'X';							//we will replace 'X' with grass when done generating houses
				}
			}
		}
		for (int i = xPos ; i < xPos+width ; i++){
			for (int j = yPos ; j < yPos+height ; j++){
				if (i == xPos || j == yPos || i == (xPos+width)-1 || j == (yPos+height)-1){	//draw houses walls if on border
					map[j][i] = wallChar;							
				} else {									//otherwise, draw floor
					map[j][i] = floorChar;
				}
			}
		}
		switch (doorWallPos){
			case 0:
				/*	#D#
				 *	#+#
				 *	###
				 */
				doorX = lround(xPos+(width/2));
				doorY = yPos;
				break;
			case 1:
				/*	###
				 *	#+#
				 *	#D#
				 */
				doorX = lround(xPos+(width/2));
				doorY = (yPos+height)-1;
				break;
			case 2:
				/*	###
				 *	D+#
				 *	###
				 */
				doorX = xPos;
				doorY = lround(yPos+(height/2));
				break;
			case 3:
				/*	###
				 *	#+D
				 *	###
				 */
				doorX = (xPos+width)-1;
				doorY = lround(yPos+(height/2));
				break;
		}
		map[doorY][doorX] = floorChar;
	}
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			if (i == 0 || j == 0 || i == 80 || j == 23){	//place invisible border around town so player doesnt leave the bounds of the screen
				map[j][i] = wallChar;
			}
			if (map[j][i] == 'X'){
				map[j][i] = '.';
			}
		}
	}
	startX = (rand() % 79)+1;
	startY = (rand() % 23)+1;
	while (map[startY][startX] != '.'){				//choose random starting player positions until an empty space is found
		startX = (rand() % 79)+1;
		startY = (rand() % 23)+1;
	}
	char tempGarden[23][80];
	int tallGrassCount = 0;
	int grassCount = 0;
	for (int i = 1 ; i < 80 ; i ++){				//randomly place thick grass (1/3 non-house tiles will be grass)
		for (int j = 1 ; j < 23 ; j++){
			if (rand() % 4 == 0 && map[j][i] == '.'){
				map[j][i] = '"';
			}
		}
	}
	for (int r = 0 ; r < 8 ; r++){					//basic cellular automata to make gardens look more natural
		for (int i = 1 ; i < 80 ; i++){
			for (int j = 1 ; j < 23 ; j++){
				tallGrassCount = 0;
				grassCount = 0;
				if (map[j-1][i] == '"'){
					tallGrassCount++;
				} else if (map[j-1][i] == '.'){
					grassCount++;
				}
				if (map[j+1][i] == '"'){
					tallGrassCount++;
				} else if (map[j+1][i] == '.'){
					grassCount++;
				}
				if (map[j][i-1] == '"'){
					tallGrassCount++;
				} else if (map[j][i-1] == '.'){
					grassCount++;
				}
				if (map[j][i+1] == '"'){
					tallGrassCount++;
				} else if (map[j][i+1] == '.'){
					grassCount++;
				}
				if (grassCount <= tallGrassCount){
					tempGarden[j][i] = '"';
				} else {
					tempGarden[j][i] = '.';
				}
				
				if (tempGarden[j-1][i] == '"' && tempGarden[j+1][i] == '"' && tempGarden[j][i-1] == '"' && tempGarden[j][i+1] == '"'){
					tempGarden[j][i] = '"';
				}
			}
		}
	}
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			if (map[j][i] == '.'){
				map[j][i] = tempGarden[j][i];
			}
		}
	}
}

char returnTownmapAt(int x, int y){
	return map[y][x];
}

int seedFromPosition(int x, int y){
	int a = abs(x);
	int b = abs(y);
	return ((a >= y) ? (a * b + a + b) : (y * y +x))+(seedMain/1000);
}

char fireValues[24][80];

void fireSpread(){
	if (canFireSpread){
		for (int i = 0 ; i < 80 ; i++){
			for (int j = 0 ; j < 24 ; j++){
				fireValues[j][i] = map[j][i];
			}
		}
		for (int i = 0 ; i < 80 ; i++){
			for (int j = 0 ; j < 24 ; j++){
				if (returnDungeonmapAt(i,j) == '*' && turn % 15 == 0){
					if (dungeonWalkable(i,j-1)){
						fireValues[j-1][i] = '*';
						fires++;
					}
					if (dungeonWalkable(i,j+1)){
						fireValues[j+1][i] = '*';
						fires++;
					}
					if (dungeonWalkable(i-1,j)){
						fireValues[j][i-1] = '*';
						fires++;
					}
					if (dungeonWalkable(i+1,j)){
						fireValues[j][i+1] = '*';
						fires++;
					}
				}
			}
		}
		for (int i = 0 ; i < 80 ; i++){
			for (int j = 0 ; j < 24 ; j++){
				map[j][i] = fireValues[j][i];
			}
		}
	}
}
