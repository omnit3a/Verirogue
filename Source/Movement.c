#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "Movement.h"
#include "Player.h"
#include "MapGenerator.h"
#include "DrawScreen.h"
#include "DrawUI.h"
#include "Main.h"

char biome;

int isSwimming;

int surroundingTemperature;

int entryX, entryY = 0;

int turn = 1;

int ch;

void drawPlayer(){
	init_pair(6,COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(6) | A_BOLD);
	if (biome == 'd'){
		mvaddch(playerEnt.currentPos.yPos, playerEnt.currentPos.xPos, '@');
	} else if (biome == 'o'){
		mvaddch(12,40,'@');
	}
	attroff(A_BOLD);
	updateScreen();
}

void getMovement(){
	ch = getch();
	int checkX = playerEnt.currentPos.xPos;
	int checkY = playerEnt.currentPos.yPos;
	char testMap = returnHeightmapAt(12,40);
	switch (ch){
		case KEY_UP:
			if (biome == 'd' && playerEnt.currentPos.yPos > 0 && returnDungeonmapAt(checkX, checkY-1) != ' '){
				playerEnt.currentPos.yPos--;
			} else if (biome == 'o' && playerEnt.currentPos.yPos > -48){
				playerEnt.currentPos.yPos--;
			}
			break;
		case KEY_DOWN:
			if (biome == 'd' && playerEnt.currentPos.yPos < 23 && returnDungeonmapAt(checkX, checkY+1) != ' '){
				playerEnt.currentPos.yPos++;
			} else if (biome == 'o' && playerEnt.currentPos.yPos < 48){
				playerEnt.currentPos.yPos++;
			}
			break;
		case KEY_LEFT:
			if (biome == 'd' && playerEnt.currentPos.xPos > 0 && returnDungeonmapAt(checkX-1, checkY) != ' '){
				playerEnt.currentPos.xPos--;
			} else if (biome == 'o' && playerEnt.currentPos.xPos > -96){
				playerEnt.currentPos.xPos--;
			}
			break;
		case KEY_RIGHT:
			if (biome == 'd' && playerEnt.currentPos.xPos < 79 && returnDungeonmapAt(checkX+1, checkY) != ' '){
				playerEnt.currentPos.xPos++;
			} else if (biome == 'o' && playerEnt.currentPos.xPos < 96){
				playerEnt.currentPos.xPos++;
			}
			break;
		case '<':
			if (biome == 'd' && map[playerEnt.currentPos.yPos][playerEnt.currentPos.xPos] == '<'){
				surroundingTemperature = 27;
				biome = 'o';
				generateMap();
				setupPlayer(entryX, entryY, 0);
				updateScreen();
				msgLog = "You left the dungeon";
			}
			break;
		case '>':
			if (biome == 'o'){
				surroundingTemperature = 15;
				entryX = playerEnt.currentPos.xPos;
				entryY = playerEnt.currentPos.yPos;
				biome = 'd';
				generateDungeon(7,7);
				setupPlayer(startX, startY,0);
				updateScreen();
				msgLog = "You found a dungeon!";
			}
			break;
		case 27:
			endScreen();
			exit(0);
	}
	turn++;
	drawTurn();
	updateScreen();
}

void updateTemperature(){
	if (turn % 25 == 0){
		if (surroundingTemperature < playerEnt.currentTemperature.celsius){
			playerEnt.currentTemperature.celsius--;
		} else if (surroundingTemperature > playerEnt.currentTemperature.celsius){
			playerEnt.currentTemperature.celsius++;
		}
		playerEnt.currentTemperature.fahrenheit = (playerEnt.currentTemperature.celsius * 9 / 5) + 32;
		drawTemperature();
		updateScreen();
	}
}
