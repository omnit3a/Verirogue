#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <string>
#include <time.h>
#include <math.h>
#include "Movement.h"
#include "Player.h"
#include "MapGenerator.h"
#include "DrawScreen.h"
#include "DrawUI.h"
#include "Main.h"
#include "Planets.h"
#include "Enemies.h"
#include "Inventory.h"
#include "Items.h"
#include "Gas.h"
#include "FileRead.h"
#include "Legacy.h"
#include "PlayerCreator.h"

char biome;

int isSwimming, onFire, hasHypothermia, hasFever = 0;

int surroundingTemperature = 37;

int entryX, entryY,fires = 0;

int turn = 1;

int ch;

char dir;		//'u' = UP 'd' = DOWN 'l' = LEFT 'r' = RIGHT

int chM, dungeonType;

int inDebugMode;

std::string foodVariant;

void updateCauterize(int x, int y){
	switch (dir){
		case 'u':
			if (fireValues[y-1][x] == '*'){
				cauterizeWound();
				msgLog = "You cauterize your wounds";
			}
			break;
		case 'd':
			if (fireValues[y+1][x] == '*'){
				cauterizeWound();
				msgLog = "You cauterize your wounds";
			}
			break;
		case 'l':
			if (fireValues[y][x-1] == '*'){
				cauterizeWound();
				msgLog = "You cauterize your wounds";
			}
			break;
		case 'r':
			if (fireValues[y][x+1] == '*'){
				cauterizeWound();
				msgLog = "You cauterize your wounds";
			}
			break;
	}
}

void engageInCombat(int x, int y){
	int damage;
	int handBonus;
	if (playerHandedness == "Left handed"){
		handBonus = -3;
	} else if (playerHandedness == "Left handed (Other)" || playerHandedness == "Right handed (Other)"){
		handBonus = -5;
	}
	switch(enemyMap[y][x]){
		case '&':
			damage = ((rand() % currentWeapon)*2)+1+handBonus;
			if (damage < 0){
				damage = 0;
			}
			enemyHealthMap[y][x] -= damage;
			msgLog = "You hit the goblin!";
			break;
		case 'k':
			damage = ((rand() % currentWeapon)*2)+1+handBonus;
			if (damage < 0){
				damage = 0;
			}
			enemyHealthMap[y][x] -= damage;
			msgLog = "You hit the kobold!";
			break;
	}
	targetPlayer(x, y);
	updateEnemyHealth();
}

void drawPlayer(){
	char playerChar;
	if (playerRace == "Human"){
		playerChar = HUMANSYM;
	} else if (playerRace == "Goblin"){
		playerChar = GOBLINSYM;
	} else if (playerRace == "Kobold"){
		playerChar = KOBOLDSYM;
	} else {
		playerChar = HUMANSYM;
	}
	init_pair(6,COLOR_YELLOW, COLOR_BLACK);
	init_pair(28, COLOR_RED, COLOR_BLACK);
	if (bloodCount < 384){
		attron(COLOR_PAIR(28) | A_BOLD);
	} else {
		attron(COLOR_PAIR(6) | A_BOLD);
	}
	if (biome == 'd' || biome == 't' || biome == 'f' || biome == 'h' || biome == 'm'){
		mvaddch(playerEnt.currentPos.yPos, playerEnt.currentPos.xPos, playerChar);
	} else if (biome == 'o'){
		mvaddch(12,40,playerChar);
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
	if (biome == 'f' || biome == 'h' || biome == 'm'){
		switch (dir){
			case 'u':
				if (playerEnt.currentPos.yPos > 0 && surroundingChar[0] != '~'){
					return 1;
				} else {
					return 0;
				}
				break;
			case 'd':
				if (playerEnt.currentPos.yPos < 23 && surroundingChar[2] != '~'){
					return 1;
				} else {
					return 0;
				}
				break;
			case 'l':
				if (playerEnt.currentPos.xPos > 0 && surroundingChar[3] != '~'){
					return 1;
				} else {
					return 0;
				}
				break;
			case 'r':
				if (playerEnt.currentPos.xPos < 79 && surroundingChar[1] != '~'){
					return 1;
				} else {
					return 0;
				}
				break;
			default:
				return 0;
		}
	} else {
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
}

int dungeonWalkable(int x, int y){
	return (map[y][x] == '.' || map[y][x] == '+' || map[y][x] == '<' || map[y][x] == '$' || map[y][x] == '*' || map[y][x] == '|' || map[y][x] == '-');
}

int canWalk(){
	int returnValue = playerEnt.leftLeg.bpHP.currentHealth > 10 || playerEnt.rightLeg.bpHP.currentHealth > 10 || ((playerEnt.leftArm.bpHP.currentHealth > 10 || playerEnt.rightArm.bpHP.currentHealth > 10) && isSwimming);
	if (returnValue == 0){
		msgLog = "You are incapable of walking!";
	}
	return returnValue;
}

int canHit(){
	int returnValue = playerEnt.leftArm.bpHP.currentHealth > 10 ||playerEnt.rightArm.bpHP.currentHealth > 10;
	if (returnValue == 0){
		msgLog = "You are incapable of attacking!";
	}
	return returnValue;
}

int inOverworld(){
	return (biome == 'o' || biome == 'f' || biome == 'h' || biome == 'm');
}

void getMovement(){
	ch = getch();
	int checkX = playerEnt.currentPos.xPos;
	int checkY = playerEnt.currentPos.yPos;
	char testMap = returnHeightmapAt(12,40);
	switch (ch){
		case KEY_UP:
			dir = 'u';
			if (isEnemyAt(checkX, checkY-1)){
				if (canHit()){
					engageInCombat(checkX, checkY-1);
				}
				break;
			}
			if (biome == 'd' && playerEnt.currentPos.yPos > 0 && dungeonWalkable(checkX, checkY-1) && canWalk()){
				playerEnt.currentPos.yPos--;
			} else if (inOverworld() && playerEnt.currentPos.yPos > -48 && canWalk()){
				if (isSwimming && !mapWalkable()){
					playerEnt.currentPos.yPos--;
				} else if (!isSwimming && mapWalkable()){
					playerEnt.currentPos.yPos--;
				}
			} else if (biome == 't' && playerEnt.currentPos.yPos > 0 && townWalkable(checkX, checkY-1) && canWalk()){
				playerEnt.currentPos.yPos--;
			}

			break;
		case KEY_DOWN:
			dir = 'd';
			if (isEnemyAt(checkX, checkY+1)){
				if (canHit()){
					engageInCombat(checkX, checkY+1);
				}
				break;
			}
			if (biome == 'd' && playerEnt.currentPos.yPos < 23 && dungeonWalkable(checkX, checkY+1) && canWalk()){
				playerEnt.currentPos.yPos++;
			} else if (inOverworld() && playerEnt.currentPos.yPos < 48 && canWalk()){
				if (isSwimming && !mapWalkable()){
					playerEnt.currentPos.yPos++;
				} else if (!isSwimming && mapWalkable()){
					playerEnt.currentPos.yPos++;
				}
			} else if (biome == 't' && playerEnt.currentPos.yPos < 23 && townWalkable(checkX, checkY+1) && canWalk()){
				playerEnt.currentPos.yPos++;
			}
			break;
		case KEY_LEFT:
			dir = 'l';
			if (isEnemyAt(checkX-1, checkY)){
				if (canHit()){
					engageInCombat(checkX-1, checkY);
				}
				break;
			}
			if (biome == 'd' && playerEnt.currentPos.xPos > 0 && dungeonWalkable(checkX-1, checkY) && canWalk()){
				playerEnt.currentPos.xPos--;
			} else if (inOverworld() && playerEnt.currentPos.xPos > -96 && canWalk()){
				if (isSwimming && !mapWalkable()){
					playerEnt.currentPos.xPos--;
				} else if (!isSwimming && mapWalkable()){
					playerEnt.currentPos.xPos--;
				}
			} else if (biome == 't' && playerEnt.currentPos.xPos > 0 && townWalkable(checkX-1, checkY) && canWalk()){
				playerEnt.currentPos.xPos--;
			}
			break;
		case KEY_RIGHT:
			dir = 'r';
			if (isEnemyAt(checkX+1, checkY)){
				if (canHit()){
					engageInCombat(checkX+1, checkY);
				}
				break;
			}
			if (biome == 'd' && playerEnt.currentPos.xPos < 79 && dungeonWalkable(checkX+1, checkY) && canWalk()){
				playerEnt.currentPos.xPos++;
			} else if (inOverworld() && playerEnt.currentPos.xPos < 96 && canWalk()){
				if (isSwimming && !mapWalkable()){
					playerEnt.currentPos.xPos++;
				} else if (!isSwimming && mapWalkable()){
					playerEnt.currentPos.xPos++;
				}
			} else if (biome == 't' && playerEnt.currentPos.xPos < 79 && townWalkable(checkX+1, checkY) && canWalk()){
				playerEnt.currentPos.xPos++;
			}
			break;
		case '<':
			if (biome == 'd' && map[playerEnt.currentPos.yPos][playerEnt.currentPos.xPos] == '<' && canWalk()){
				surroundingTemperature = 37;
				biome = 'o';
				generateMap();
				playerEnt.currentPos.xPos = entryX;
				playerEnt.currentPos.yPos = entryY;
				resetEnemies(0);
				updateScreen();
				msgLog = "You left the dungeon";
				fires = 0;
				dungeonHasFire = 0;
			} else if (biome != 'o' && inOverworld() && canWalk){
				surroundingTemperature = 37;
				biome = 'o';
				generateMap();
				playerEnt.currentPos.xPos = entryX;
				playerEnt.currentPos.yPos = entryY;
				updateScreen();
				msgLog = "You return to travelling";
				fires = 0;
			} else if (biome == 't' && canWalk()){
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
			srand(time(0));
			dungeonType = rand() % 2;	
			if (biome == 'o' && underPlayer == '>' && canWalk()){
				entryX = checkX;
				entryY = checkY;
				biome = 'd';
				surroundingTemperature = 15;
				generateDungeon(7,7);
				playerEnt.currentPos.xPos = startX;
				playerEnt.currentPos.yPos = startY;
				updateScreen();
				msgLog = "You enter the dungeon";
			} else if (biome == 'o' && canWalk() && underPlayer != '>'){
				entryX = checkX;
				entryY = checkY;
				generateCloseUp();
				playerEnt.currentPos.xPos = 40;
				playerEnt.currentPos.yPos = 12;
				updateScreen();
			} else {
				msgLog = "There isn't a dungeon here!";
			}
			break;
		case 'f':
			if (biome == 'd' && canHit()){
				switch(dir){
					case 'u':
						if (dungeonWalkable(checkX,checkY-1) && returnDungeonmapAt(checkX, checkY-1) != '<'){
							map[checkY-1][checkX] = '*';
							dungeonHasFire = 1;
							msgLog = "You lit a fire";
						}
						break;
					case 'd':
						if (dungeonWalkable(checkX, checkY+1) && returnDungeonmapAt(checkX, checkY+1) != '<'){
							map[checkY+1][checkX] = '*';
							dungeonHasFire = 1;
							msgLog = "You lit a fire";
						}
						break;
					case 'l':
						if (dungeonWalkable(checkX-1, checkY) && returnDungeonmapAt(checkX-1, checkY) != '<'){
							map[checkY][checkX-1] = '*';
							dungeonHasFire = 1;
							msgLog = "You lit a fire";
						}
						break;
					case 'r':
						if (dungeonWalkable(checkX+1, checkY) && returnDungeonmapAt(checkX+1, checkY) != '<'){
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
			if ((surroundingChar[0] == '~' || surroundingChar[1] == '~' || surroundingChar[2] == '~' || surroundingChar[3] == '~') && underPlayer != 'A'){
				foodVariant = "Shellfish";
			} else if (underPlayer == 'A'){
				foodVariant = "Wintergreen";
			} else {
				foodVariant = "Berries";
			}
			if ((biome == 'o' || biome == 't')&& rand() % 2 == 0 && itemCount < 16){
				msgLog = "You scavenged for "+foodVariant;
				inventory[itemCount] = foodVariant;

				//set the starting rot level of food
				if (foodVariant == "Wintergreen"){
					itemTurn[itemCount] = 0;
				} else if (foodVariant == "Berries"){
					itemTurn[itemCount] = 50;
				} else if (foodVariant == "Shellfish"){
					itemTurn[itemCount] = 100;
				}
				
				itemCount++;
			} else if (itemCount >= 16){
				msgLog = "Your can't carry anything else!";
			} else {
				msgLog = "You didn't find any food";
			}
			break;
		case 'S':
			drawSheet();
			break;
		case 'e':
			if (hasFood() && foodScore+5 < 155){
				consumeFood();
				msgLog = "You ate some food";
				foodEaten++;
				if (foodScore+5 < 155){
					foodScore+=5;
					if (playerEnt.head.bpHP.currentHealth+5 < 100){
						playerEnt.head.bpHP.currentHealth += 5;
					}
					if (playerEnt.torso.bpHP.currentHealth+5 < 100){
						playerEnt.torso.bpHP.currentHealth += 5;
					}
					if (playerEnt.leftLeg.bpHP.currentHealth+5 < 100){
						playerEnt.leftLeg.bpHP.currentHealth += 5;
					}
					if (playerEnt.rightLeg.bpHP.currentHealth+5 < 100){
						playerEnt.rightLeg.bpHP.currentHealth += 5;
					}
					if (playerEnt.leftArm.bpHP.currentHealth+5 < 100){
						playerEnt.leftArm.bpHP.currentHealth += 5;
					}
					if (playerEnt.rightArm.bpHP.currentHealth+5 < 100){
						playerEnt.rightArm.bpHP.currentHealth += 5;
					}
					if (playerEnt.skin.bpHP.currentHealth+15 < 115){
						playerEnt.skin.bpHP.currentHealth += 15;
					}
					for (int i = 0 ; i < 50 ; i++){
						if (bloodLossRate-1 > -1){
							bloodLossRate--;
						} else {
							break;
						}	
					}
					for (int i = 0 ; i < 50 ; i++){
						if (bloodCount+1 <= 511){
							bloodCount++;
						} else {
							break;
						}
					}
				} else {
					msgLog = "You are too full to eat anything";
				}
			} else if (foodScore+5 >= 155){
				msgLog = "You are too full to eat anything";
				gluttonCount++;
			} else {
				msgLog = "You don't have any food";
			}
			break;
		case 'm':
			if (inOverworld() && (hour < 6 || hour >= 20)){
				drawStarmap();
				updateScreen();
				getch();
			} else if (!inOverworld()){
				msgLog = "You can't see the sky at the moment";
			} else {
				msgLog = "The stars aren't out right now";
			}
			break;
		case 'p':
			msgLog = "You are now prone!";
			isSwimming = !isSwimming;
			break;
		case 'q':
			if (biome == 'o' && !mapWalkable()){
				playerEnt.currentHydration.hydration += 5;
			}
			break;
		case 'Q':
			quaffPotion();
			potionsQuaffed++;
			break;
		case '?':
			drawHelp();
			break;
		case 'i':
			drawInventory();
			break;
		case ',':
			if (itemAt(checkX, checkY) != ' ' && itemCount < 16){
				itemsFound++;
				switch (itemAt(checkX, checkY)){
					case SWORDSYM:
						msgLog = "You found a sword";
						inventory[itemCount] = "Sword";
						itemMap[checkY][checkX] = ' ';
						itemCount++;
						break;
					case AXESYM:
						msgLog = "You found an axe";
						inventory[itemCount] = "Axe";
						itemMap[checkY][checkX] = ' ';
						itemCount++;
						break;
					case SCROLLSYM:
						msgLog = "You found a scroll!";
						inventory[itemCount] = "Scroll";
						itemMap[checkY][checkX] = ' ';
						itemCount++;
						break;
					case POTIONSYM:
						msgLog = "You found a potion!";
						inventory[itemCount] = "Potion";
						itemMap[checkY][checkX] = ' ';
						itemCount++;
						break;
					case BERRYSYM:
						msgLog = "You found berries!";
						inventory[itemCount] = "Berries";
						itemMap[checkY][checkX] = ' ';
						itemCount++;
						break;
					case WINTERSYM:
						msgLog = "You found wintergreen!";
						inventory[itemCount] = "Wintergreen";
						itemMap[checkY][checkX] = ' ';
						itemCount++;
						break;
					case SHELLFISHSYM:
						msgLog = "You found shellfish!";
						inventory[itemCount] = "Shellfish";
						itemMap[checkY][checkX] = ' ';
						itemCount++;
						break;
					case ROTTENSYM:
						msgLog = "You found rotten food";
						inventory[itemCount] = "Rotten food";
						itemMap[checkY][checkX] = ' ';
						itemCount++;
						break;
					default: 
						msgLog = "You're not sure what this is";
						break;
				}
			} else if (itemCount >= 16){
				msgLog = "You can't carry anything else!";
			} else {
				msgLog = "There isn't anything to pickup!";
			}
			if (biome == 'd' && returnDungeonmapAt(checkX, checkY) == '$'){
				map[checkY][checkX] = '.';
				srand(time(0));
				int randVal = (rand() % 24)+1;
				goldScore += randVal;
				goldFound += randVal;
				msgLog = "You found some gold!";
			}
			break;
		case 'd':
			drawDropScreen();
			drawDungeon();
			break;
		case 'r':
			readScroll();
			break;
		case 'M':
			init_pair(30, COLOR_WHITE, COLOR_BLACK);
			clear();
			attron(COLOR_PAIR(30) | A_BOLD);
			mvprintw(0,0,"Do what?");
			attron(A_BLINK);
			mvaddch(1,0,'>');
			attroff(A_BOLD | A_BLINK);
			refresh();
			chM = getch();
			switch (chM){
				case 'c':
					updateCauterize(checkX, checkY);
					break;
				case 'd':
					if (turn-diseaseStart < 50 && isDiseased == 1){
						srand(time(0));	
						if (rand() % 3 == 0){
							isDiseased = 0;
							msgLog = "You cured your disease";
							medicalSuccesses++;
						} else {
							msgLog = "You failed to cure your disease";
							medicalFailures--;
						}
					} else if (isDiseased == 0){
						msgLog = "You don't have a disease";
					}
					break;
				case 'C':
					//clean floor
					bloodMap[checkY][checkX] = 0;
					break;
				default:
					msgLog = "You do nothing";
					break;
			}
			break;
		case 'D':
			switch (returnDungeonmapAt(checkX, checkY)){
				case '+':
					msgLog = "It's a stone floor";
					break;
				case '.':
					msgLog = "It's a wooden plank floor";
					break;
				case '<':
					msgLog = "It's an upward staircase";
					break;
			}
			if (biome == 'o'){
				switch (underPlayer){
					case '.':
						msgLog = "It's an open field";
						break;
					case '~':
						msgLog = "It's a lake";
						break;
					case '>':
						msgLog = "It's a downard staircase";
						break;
					case '^':
						msgLog = "It's some forested terrain";
						break;
					case 'A':
						msgLog = "It's some mountainous terrain";
						break;
					case 't':
						msgLog = "It's a small patch of trees";
						break;
				}
			}
			if (enemyMap[checkY-1][checkX] == '&' || enemyMap[checkY+1][checkX] == '&' || enemyMap[checkY][checkX-1] == '&' || enemyMap[checkY][checkX+1] == '&'){
				msgLog = "It's a goblin";
			} else if (enemyMap[checkY-1][checkX] == 'k' || enemyMap[checkY+1][checkX] == 'k' || enemyMap[checkY][checkX-1] == 'k' || enemyMap[checkY][checkX+1] == 'k'){
				msgLog = "It's a kobold";
			}

			if (enemyDiseaseMap[checkY-1][checkX] == 1 || enemyDiseaseMap[checkY+1][checkX] == 1 || enemyDiseaseMap[checkY][checkX-1] == 1 || enemyDiseaseMap[checkY][checkX+1] == 1){
				if (enemyMap[checkY-1][checkX] == '&' || enemyMap[checkY+1][checkX] == '&' || enemyMap[checkY][checkX-1] == '&' || enemyMap[checkY][checkX+1] == '&'){
					msgLog = "It's a diseased goblin";
				} else if (enemyMap[checkY-1][checkX] == 'k' || enemyMap[checkY+1][checkX] == 'k' || enemyMap[checkY][checkX-1] == 'k' || enemyMap[checkY][checkX+1] == 'k'){
					msgLog = "It's a diseased kobold";
				}
			}
			switch(itemMap[checkY][checkX]){
				case SWORDSYM:
					msgLog = "It's a shiny new sword!";
					break;
				case AXESYM:
					msgLog = "It's a shiny new axe!";
					break;
				case SCROLLSYM:
					msgLog = "It's a scroll";
					break;
				case POTIONSYM:
					msgLog = "It's a magical potion!";
					break;
				case BERRYSYM:
					msgLog = "It's a pile of berries";
					break;
				case WINTERSYM:
					msgLog = "It's a pile of wintergreen leaves";
					break;
				case SHELLFISHSYM:
					msgLog = "It's a pile of shellfish";
					break;
				case ROTTENSYM:
					msgLog = "It's a pile of rotten food";
					break;
			}
			break;
		case 'x':
			if (playerHandedness == "Left handed"){
				playerHandedness = "Left handed (Other)";
			} else if (playerHandedness == "Right handed"){
				playerHandedness = "Right handed (Other)";
			} else if (playerHandedness == "Left handed (Other)"){
				playerHandedness = "Left handed";
			} else if (playerHandedness == "Right handed (Other)"){
				playerHandedness = "Right handed";
			}
			break;
		case 27:
			killPlayer("Thank you for playing!");
			break;
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
	if (turn % 33 == 0 && biome == 'o'){
		scrollStars();
		setDayNight();
	}
	pseudoPathfind();
	bleedCheck();
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
	if (underPlayer == '~'){
		surroundingTemperature = 8;
	}
	if (turn % 25 == 0){
		if (surroundingTemperature < playerEnt.currentTemperature.celsius){
			playerEnt.currentTemperature.celsius--;
		} else if (surroundingTemperature > playerEnt.currentTemperature.celsius){
			playerEnt.currentTemperature.celsius++;
		}
		if (hasFever == 1){
			playerEnt.currentHydration.hydration-=2;
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
		killPlayer("You died of overheating! Be more careful next time.");
	} else if (playerEnt.currentTemperature.celsius < 14){
		killPlayer("You froze to death! Be more careful next time.");
	}
	if (turn % 150 == 0){
		playerEnt.currentHydration.hydration -= 5;
	}
	if (playerEnt.currentHydration.hydration < 25){
		killPlayer("You died of thirst! Be more careful next time.");
	} else if (playerEnt.currentHydration.hydration >= 80){
		killPlayer("You died of Hyponatremia! Be more careful next time.");
	}
	if (fireValues[playerEnt.currentPos.yPos][playerEnt.currentPos.xPos] == '*'){
		playerEnt.skin.bpHP.currentHealth -= 10;
		msgLog = "Your skin melts!";
	}
	if (playerEnt.skin.bpHP.currentHealth <= 0){
		killPlayer("Your skin melted off! Be more careful next time.");
	}
}

void updateHunger(){
	if (turn % 200 == 0){
		foodScore-=5;
		if (foodScore < 10){
			killPlayer("You died of Hunger! Be more careful next time.");
		}
	}
}
