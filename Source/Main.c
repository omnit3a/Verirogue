#include <stdio.h>
#include <stdlib.h>
#include "MapGenerator.h"
#include "DrawScreen.h"
#include "Player.h"
#include <ncurses.h>
#include <time.h>
#include "Movement.h"
#include "DrawUI.h"

int seedMain = time(0);

int main(){
	setupScreen();
	updateScreen();
	generateMap();
	setupPlayer(0,0,0);
	biome = 'o';
	while(1){
		if (biome == 'o'){
			generateMap();
			drawMap();
			updateTemperature();
			updateHunger();
			drawUserInterface();
			drawPlayer();
			getMovement();
			updateScreen();
		} else if (biome == 'd'){
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
	}
	getch();
	endScreen();
	return 0;
}
