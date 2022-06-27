#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "MapGenerator.h"
#include "Movement.h"
#include "Player.h"

char gasMap[24][80];
int gasValues[24][80];

int initializedGas = 0;

void initGas(){
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			gasMap[j][i] = ' ';
			gasValues[j][i] = 0;
		}
	}
	initializedGas = 1;
}

void placeGas(int x, int y, char gasType){
	if (initializedGas == 0){
		initGas();
	}
	gasMap[y][x] = gasType;
	gasValues[y][x] = 10;
}

void diffuseGas(int x, int y){
	char tempGasMap[24][80];
	char tempGasValues[24][80];
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			tempGasMap[j][i] = gasMap[j][i];
			tempGasValues[j][i] = gasValues[j][i];
		}
	}
	int count;
	int average;
	char diffusionType;
	count = 0;
	diffusionType = tempGasMap[y][x];
	for (int i = x-1 ; i < x+2 ; i++){
		for (int j = y-1 ; j < y+2 ; j++){
			if (i > 0 && i < 79 && j > 0 && j < 23){
				count += tempGasValues[j][i];
			}
		}
	}
	average = lround(count/8);
	tempGasValues[y][x] = average;
	if (average <= 0){
		tempGasMap[y][x] = ' ';
	} else {
		tempGasMap[y][x] = diffusionType;
	}
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			gasMap[j][i] = tempGasMap[j][i];
			gasValues[j][i] = tempGasValues[j][i];
		}
	}
}
