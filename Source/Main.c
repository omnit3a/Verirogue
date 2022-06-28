#include <stdio.h>
#include <stdlib.h>
#include "MapGenerator.h"
#include "DrawScreen.h"
#include "Player.h"
#include <ncurses.h>
#include <time.h>
#include "Movement.h"
#include "DrawUI.h"
#include "PlayerCreator.h"
#include "Planets.h"
#include "Enemies.h"
#include "FileRead.h"
#include "Inventory.h"
#include "Gas.h"

int seedMain = time(0);

int main(){
	initFileRead();
	worldFileRead();
	initGas();
	setupScreen();
	updateScreen();
	playerCreatorScreen();
	generateMap();
	generatePlanetaryBodies(5,80);
	setupPlayer(0,0,0);
	resetEnemies(0);
	biome = 'o';
	while(1){
		if (biome == 'o'){
			generateMap();
			resetEnemies(0);				//makes sure no enemies spawn on the overworld
			drawMap();
			updateTemperature();
			updateHunger();
			setDayNight();
			drawUserInterface();
			drawPlayer();
			getMovement();
			updateScreen();
		} else if (biome == 'd'){
			resetEnemies(1);
			drawDungeon();
			updateTemperature();
			updateHunger();
			drawUserInterface();
			drawPlayer();
			fireSpread();
			getMovement();
			updateScreen();
		} else if (biome == 't'){
			drawTown();
			updateTemperature();
			updateHunger();
			drawUserInterface();
			drawPlayer();
			getMovement();
			updateScreen();
		}
		killCheck();
		infectionCheck();
		rotItems();
	}
	getch();
	endScreen();
	return 0;
}
