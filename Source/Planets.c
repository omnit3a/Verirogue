#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>
#include "Player.h"
#include "DrawUI.h"

char skyMap[24][80];

int sunX, sunY;

int month, day, hour;

int calender[12][30];

int isDay = 1;

void generatePlanetaryBodies(int maxPlanets, int maxStars){
	
	//initialize stuff
	srand(time(0)+lround(time(0)/25));
	int planetCount = (rand() % maxPlanets)+1;
	int starCount = (rand() % maxStars)+1;
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			skyMap[j][i] = ' ';
		}
	}
	int starX, starY;

	//generate stars
	for (int i = 0 ; i < starCount ; i++){
		starX = rand() % 80;
		starY = rand() % 24;
		skyMap[starY][starX] = '.';
	}
	sunX = 40;
	sunY = 12;
	skyMap[sunY][sunX] = 'X';

	int planetX, planetY;

	//generate planets
	int planetTries = 0;
	for (int i = 0 ; i < planetCount && planetTries < 256; i++){
		planetX = rand() % 80;
		planetY = rand() % 24;
		if (skyMap[planetY][planetX] == ' '){
			skyMap[planetY][planetX] = '#';
		} else {
			planetTries++;
			continue;
		}
	}
	month = 1;
	day = 1;
	hour = 12;
}

void scrollStars(){
	for (int i = 0 ; i < 24 ; i++){
		skyMap[i][79] = skyMap[i][0];
	}
	for (int i = 0 ; i < 79 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			skyMap[j][i] = skyMap[j][i+1];
		}
	}
	sunX--;
	if (sunX < 0){
		sunX = 79;
	}
	hour++;
	if (hour > 24){
		hour = 1;
		day++;
		if (day > 30){
			day = 1;
			month++;
			if (month > 12){
				month = 1;
			}
		}
	}
}

void setDayNight(){
	if (hour > 6 && hour < 20){
		if (isDay == 0){
			msgLog = "It is now day";
		}
		isDay = 1;
		
	} else {
		if (isDay == 1){
			msgLog = "It is now night";
		}
		isDay = 0;
	}
}
