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

FastNoiseLite noise;

int dungeonHasFire = 0;

int returnCoastmapAt(int x, int y){
	noise.SetFrequency(0.05);
	noise.SetSeed(seedMain);
	noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
	int value = abs(lround((noise.GetNoise((float) playerEnt.currentPos.xPos+x, (float) playerEnt.currentPos.yPos+y)+0.5)/0.10));
	if (value < 1){
		return 0;
	} else {
		return value;
	}
}

char map[24][80];

void generateMap(){
	noise.SetFrequency(0.1);
	noise.SetSeed(seedMain);
	noise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
	noise.SetCellularDistanceFunction(FastNoiseLite::CellularDistanceFunction_Manhattan);
	int value;
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			map[j][i] = '~';
		}
	}
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			value = abs(lround((noise.GetNoise((float) playerEnt.currentPos.xPos+i, (float) playerEnt.currentPos.yPos+j)+0.5)/0.10));
			if (playerEnt.currentPos.xPos+i > 136 || playerEnt.currentPos.xPos+i < -56 || playerEnt.currentPos.yPos+j > 60 || playerEnt.currentPos.yPos+j < -36){
				map[j][i] = ' ';
				continue;
			}
			if (returnCoastmapAt(i,j) != 0 && value > 3){
				if (value == 11 || value == 12){
					map[j][i] = '^';
				} else if (value > 12){
					map[j][i] = 'A';
				} else {
					map[j][i] = '.';
				}
			} else {
				map[j][i] = '~';
			}
		}
	}
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

void generateDungeon(int maxWidth, int maxHeight){
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			seenMap[j][i] = ' ';
		}
	}
	for (int i = 0 ; i < 24 ; i++){
		for (int j = 0 ; j < 80 ; j++){
			map[i][j] = ' ';
		}
	}
	srand(seedFromPosition(entryX, entryY));
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
		startX = rand() % 80;
		startY = rand() % 24;
		if (map[startY][startX] == ' '){
			goto end;
		} else {
			map[startY][startX] = '<';
		}
}

char returnDungeonmapAt(int x, int y){
	return map[y][x];
}

int seedFromPosition(int x, int y){
	int a = abs(x);
	int b = abs(y);
	return ((a >= y) ? (a * b + a + b) : (y * y +x))+(seedMain/1000000);
}
