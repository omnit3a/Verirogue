#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include <math.h>
#include "Movement.h"
#include "Player.h"
#include "MapGenerator.h"
#include "DrawScreen.h"
#include "DrawUI.h"
#include "Main.h"
#include "Planets.h"

char biome;

int isSwimming, onFire, hasHypothermia, hasFever = 0;

int surroundingTemperature = 37;

int entryX, entryY,fires = 0;

int turn = 1;

int ch;

char dir;		//'u' = UP 'd' = DOWN 'l' = LEFT 'r' = RIGHT

void drawPlayer(){
	init_pair(6,COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(6) | A_BOLD);
	if (biome == 'd' || biome == 't'){
		mvaddch(playerEnt.currentPos.yPos, playerEnt.currentPos.xPos, '@');
	} else if (biome == 'o'){
		mvaddch(12,40,'@');
	}
	attroff(A_BOLD);
	updateScreen();
}

int townWalkable(int x, int y){
	if (returnTownmapAt(x, y) != ' ' && returnTownmapAt(x, y) != '#'){
		return 1;
	} else {
		return 0;
	}
}

int mapWalkable(){
	switch (dir){
		case 'u':
			if (surroundingChar[0] != '~'){
				return 1;
			} else {
				return 0;
			}
			break;
		case 'd':
			if (surroundingChar[2] != '~'){
				return 1;
			} else {
				return 0;
			}
			break;
		case 'l':
			if (surroundingChar[3] != '~'){
				return 1;
			} else {
				return 0;
			}
			break;
		case 'r':
			if (surroundingChar[1] != '~'){
				return 1;
			} else {
				return 0;
			}
			break;
		default:
			return 0;
	}
}

void getMovement(){
	ch = getch();
	int checkX = playerEnt.currentPos.xPos;
	int checkY = playerEnt.currentPos.yPos;
	char testMap = returnHeightmapAt(12,40);
	switch (ch){
		case KEY_UP:
			dir = 'u';
			if (biome == 'd' && playerEnt.currentPos.yPos > 0 && returnDungeonmapAt(checkX, checkY-1) != ' '){
				playerEnt.currentPos.yPos--;
			} else if (biome == 'o' && playerEnt.currentPos.yPos > -48){
				if (isSwimming && !mapWalkable()){
					playerEnt.currentPos.yPos--;
				} else if (!isSwimming && mapWalkable()){
					playerEnt.currentPos.yPos--;
				}
			} else if (biome == 't' && playerEnt.currentPos.yPos > 0 && townWalkable(checkX, checkY-1)){
				playerEnt.currentPos.yPos--;
			}
			break;
		case KEY_DOWN:
			dir = 'd';
			if (biome == 'd' && playerEnt.currentPos.yPos < 23 && returnDungeonmapAt(checkX, checkY+1) != ' '){
				playerEnt.currentPos.yPos++;
			} else if (biome == 'o' && playerEnt.currentPos.yPos < 48){
				if (isSwimming && !mapWalkable()){
					playerEnt.currentPos.yPos++;
				} else if (!isSwimming && mapWalkable()){
					playerEnt.currentPos.yPos++;
				}
			} else if (biome == 't' && playerEnt.currentPos.yPos < 23 && townWalkable(checkX, checkY+1)){
				playerEnt.currentPos.yPos++;
			}
			break;
		case KEY_LEFT:
			dir = 'l';
			if (biome == 'd' && playerEnt.currentPos.xPos > 0 && returnDungeonmapAt(checkX-1, checkY) != ' '){
				playerEnt.currentPos.xPos--;
			} else if (biome == 'o' && playerEnt.currentPos.xPos > -96){
				if (isSwimming && !mapWalkable()){
					playerEnt.currentPos.xPos--;
				} else if (!isSwimming && mapWalkable()){
					playerEnt.currentPos.xPos--;
				}
			} else if (biome == 't' && playerEnt.currentPos.xPos > 0 && townWalkable(checkX-1, checkY)){
				playerEnt.currentPos.xPos--;
			}
			break;
		case KEY_RIGHT:
			dir = 'r';
			if (biome == 'd' && playerEnt.currentPos.xPos < 79 && returnDungeonmapAt(checkX+1, checkY) != ' '){
				playerEnt.currentPos.xPos++;
			} else if (biome == 'o' && playerEnt.currentPos.xPos < 96){
				if (isSwimming && !mapWalkable()){
					playerEnt.currentPos.xPos++;
				} else if (!isSwimming && mapWalkable()){
					playerEnt.currentPos.xPos++;
				}
			} else if (biome == 't' && playerEnt.currentPos.xPos < 79 && townWalkable(checkX+1, checkY)){
				playerEnt.currentPos.xPos++;
			}
			break;
		case '<':
			if (biome == 'd' && map[playerEnt.currentPos.yPos][playerEnt.currentPos.xPos] == '<'){
				surroundingTemperature = 37;
				biome = 'o';
				generateMap();
				playerEnt.currentPos.xPos = entryX;
				playerEnt.currentPos.yPos = entryY;
				updateScreen();
				msgLog = "You left the dungeon";
				fires = 0;
				dungeonHasFire = 0;
			} else if (biome == 't'){
				surroundingTemperature = 37;
				biome = 'o';
				generateMap();
				playerEnt.currentPos.xPos = entryX;
				playerEnt.currentPos.yPos = entryY;
				updateScreen();
				msgLog = "You left the town";
				fires = 0;
			}
			break;
		case '>':
			entryX = checkX;
			entryY = checkY;
			srand(time(0));
			if (biome == 'o' && underPlayer == '>'){
				biome = 'd';
				surroundingTemperature = 15;
				generateDungeon(7,7);
				playerEnt.currentPos.xPos = startX;
				playerEnt.currentPos.yPos = startY;
				updateScreen();
				msgLog = "You enter the dungeon!";
			} else {
				msgLog = "There isn't a dungeon here!";
			}
			break;
		case 'f':
			if (biome == 'd'){
				switch(dir){
					case 'u':
						if (returnDungeonmapAt(checkX, checkY-1) != ' ' && returnDungeonmapAt(checkX, checkY-1) != '<'){
							map[checkY-1][checkX] = '*';
							dungeonHasFire = 1;
							msgLog = "You lit a fire";
						}
						break;
					case 'd':
						if (returnDungeonmapAt(checkX, checkY+1) != ' ' && returnDungeonmapAt(checkX, checkY+1) != '<'){
							map[checkY+1][checkX] = '*';
							dungeonHasFire = 1;
							msgLog = "You lit a fire";
						}
						break;
					case 'l':
						if (returnDungeonmapAt(checkX-1, checkY) != ' ' && returnDungeonmapAt(checkX-1, checkY) != '<'){
							map[checkY][checkX-1] = '*';
							dungeonHasFire = 1;
							msgLog = "You lit a fire";
						}
						break;
					case 'r':
						if (returnDungeonmapAt(checkX+1, checkY) != ' ' && returnDungeonmapAt(checkX+1, checkY) != '<'){
							map[checkY][checkX+1] = '*';
							dungeonHasFire = 1;
							msgLog = "You lit a fire";
						}
						break;
				}
			} else {
				msgLog = "There's no need for that now!";
			}
			break;
		case 's':
			srand(time(0));
			if ((biome == 'o' || biome == 't')&& rand() % 5 == 0){
				foodCount++;
				msgLog = "You scavenged for food";
			} else {
				msgLog = "You didn't find any food";
			}
			break;
		case 'S':
			drawSheet();
			break;
		case 'e':
			if (foodCount > 0){
				foodCount--;
				msgLog = "You ate some food";
				if (foodScore+5 < 155){
					foodScore+=5;
				} else {
					msgLog = "You are too full to eat anything";
				}
			} else {
				msgLog = "You don't have any food!";
			}
			break;
		case '$':
			if (biome == 'd' && returnDungeonmapAt(checkX, checkY) == '$'){
				map[checkY][checkX] = '.';
				srand(time(0));
				goldScore += rand() % 25;
			}
			break;
		case 'k':
			if (biome == 'd'){
				switch (dir){
					case 'u':
						if (returnDungeonmapAt(checkX, checkY-1) == ' ' && returnDungeonmapAt(checkX, checkY-2) != ' ' && checkY > 1){
							map[checkY-1][checkX] = '|';
							msgLog = "You kicked down a wall!";
						} else {
							msgLog = "OUCH! That hurt!";
						}
						break;
					case 'd':
						if (returnDungeonmapAt(checkX, checkY+1) == ' ' && returnDungeonmapAt(checkX, checkY+2) != ' ' && checkY < 22){
							map[checkY+1][checkX] = '|';
							msgLog = "You kicked down a wall!";
						} else {
							msgLog = "OUCH! That hurt!";
						}
						break;
					case 'l':
						if (returnDungeonmapAt(checkX-1, checkY) == ' ' && returnDungeonmapAt(checkX-2, checkY) != ' ' && checkX > 1){
							map[checkY][checkX-1] = '-';
							msgLog = "You kicked down a wall!";
						} else {
							msgLog = "OUCH! That hurt!";
						}
						break;
					case 'r':
						if (returnDungeonmapAt(checkX+1, checkY) == ' ' && returnDungeonmapAt(checkX+2, checkY) != ' ' && checkX < 78){
							map[checkY][checkX+1] = '-';
							msgLog = "You kicked down a wall!";
						} else {
							msgLog = "OUCH! That hurt!";
						}
						break;
				}
			}
			break;
		case 'm':
			if (biome == 'o'){
				clear();
				drawStarmap();
				updateScreen();
				getch();
			} else {
				msgLog = "You can't see the sky at the moment";
			}	
			break;
		case 'p':
			msgLog = "You are now prone!";
			isSwimming = !isSwimming;
			break;
		case 27:
			endScreen();
			exit(0);
	}
	turn++;
	drawTurn();
	if (checkY == -48 || checkY == 48 || checkX == -96 || checkX == 96 && biome == 'o'){
		msgLog = "You stare into the abyss...";
	}

	/*the star position changes every 100 turns
	 *the time of day will be effected by the star position
	 *	
	 */
	if (turn % 100 == 0 && biome == 'o'){
		scrollStars();
		setDayNight();
	}
	updateScreen();
}

void updateTemperature(){
	if (biome == 'd' && turn % 10 == 0){
		for (int i = 0 ; i < 80 ; i++){
			for (int j = 0 ; j < 24 ; j++){
				if (returnDungeonmapAt(i,j) == '*'){
					fires++;
				}
			}
		}
		if (fires > 0){
			surroundingTemperature=15+(fires*fires);
		} else {
			dungeonHasFire = 0;
		}
	}
	if (turn % 25 == 0){
		if (surroundingTemperature < playerEnt.currentTemperature.celsius){
			playerEnt.currentTemperature.celsius--;
		} else if (surroundingTemperature > playerEnt.currentTemperature.celsius){
			playerEnt.currentTemperature.celsius++;
		}
		if (hasFever == 1){
			playerEnt.currentHydration.hydration-=2;
		} else if (hasFever == 0 && playerEnt.currentHydration.hydration < 37){
			playerEnt.currentHydration.hydration++;
		}
		playerEnt.currentTemperature.fahrenheit = (playerEnt.currentTemperature.celsius * 9 / 5) + 32;
		drawTemperature();
		updateScreen();
	}
	if (playerEnt.currentTemperature.celsius < 35){
		hasHypothermia = 1;
	} else if (playerEnt.currentTemperature.celsius > 39){
		hasFever = 1;
	} else {
		hasFever = 0;
		hasHypothermia = 0;
	}
	if (playerEnt.currentTemperature.celsius >= 44){
		clear();
		mvprintw(12, 40, "You died of overheating! Be more careful next time");
		updateScreen();
		endScreen();
		exit(0);
	} else if (playerEnt.currentTemperature.celsius < 14){
		clear();
		mvprintw(12, 40, "You froze to death! Be more careful next time");
		updateScreen();
		endScreen();
		exit(0);
	}
	if (playerEnt.currentHydration.hydration < 40){
		clear();
		mvprintw(12,40, "You died of thirst! Be more careful next time");
		updateScreen();
		endScreen();
		exit(0);
	} else if (playerEnt.currentHydration.hydration >= 80){
		clear();
		mvprintw(12,40,"You died of Hyponatremia! Be more careful next time");
		updateScreen();
		endScreen();
		exit(0);
	}
}

void updateHunger(){
	if (turn % 75 == 0){
		foodScore-=5;
		if (foodScore < 10){
			clear();
			mvprintw(12,40,"You died of Hunger! Be more careful next time");
			updateScreen();
			endScreen();
			exit(0);
		}
	}
}
