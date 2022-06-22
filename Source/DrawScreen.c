#include <stdio.h>
#include <stdlib.h>
#include "DrawScreen.h"
#include "MapGenerator.h"
#include <ncurses.h>
#include <time.h>
#include "Player.h"
#include "Movement.h"
#include "Planets.h"
#include "Enemies.h"

#define WIDTH 80
#define HEIGHT 24

void setupScreen(){
	initscr();
	raw();
	noecho();
	keypad(stdscr,TRUE);
	if (has_colors() == 0){
		endwin();
		printf("Your terminal does not support color\n");
		exit(1);
	}
	start_color();
	curs_set(0);
	clear();
}

void updateScreen(){
	refresh();
}

void endScreen(){
	endwin();
}

void drawMap(){
	biome = 'o';
	init_pair(1, COLOR_CYAN, COLOR_BLACK);
	init_pair(2, COLOR_GREEN, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
	init_pair(6, COLOR_YELLOW, COLOR_BLACK);
	char currentChar;
	for (int i = 0 ; i < WIDTH ; i++){
		for (int j = 0 ; j < HEIGHT ; j++){
			mvaddch(j,i,' ');
		}
	}
	for (int i = 0 ; i < WIDTH ; i++){
		for (int j = 0 ; j < HEIGHT ; j++){
			currentChar = returnHeightmapAt(i,j);
			if (currentChar == '^' || currentChar == '.'){
				attron(COLOR_PAIR(2));
			} else if (currentChar == 'A' || currentChar == '*' || currentChar == '>'){
				attron(COLOR_PAIR(3));
			} else if (currentChar == 't'){
				attron(COLOR_PAIR(2) | A_BOLD);
			} else{
				attron(COLOR_PAIR(1));
			}
			mvaddch(j,i,returnHeightmapAt(i,j));
			attroff(A_BOLD);
		}
	}
	updateScreen();
	
}

void drawCountries(){
	clear();
	init_pair(1, COLOR_BLACK, COLOR_WHITE);
	attron(COLOR_PAIR(1));
	for (int i = 0 ; i < WIDTH ; i++){
		for (int j = 0 ; j < HEIGHT ; j++){
			mvaddch(j,i,returnCountryAt(i,j));
		}
	}
	updateScreen;
}

char seenMap[24][80];

void drawWithoutFOV(){
	init_pair(1, COLOR_WHITE , COLOR_BLACK);
	init_pair(2, COLOR_YELLOW , COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(10, COLOR_RED, COLOR_BLACK);
	init_pair(23, COLOR_MAGENTA, COLOR_BLACK);
	clear();
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			char currentChar = map[j][i];
			attron(COLOR_PAIR(1));
			if (currentChar == '.'){
				attron(COLOR_PAIR(2));
			} else if (currentChar == '+' || currentChar == '|' || currentChar == '-'){
				attron(COLOR_PAIR(1));	
			} else if (currentChar == '<'){
				attron(COLOR_PAIR(1));
			} else if (currentChar == '*'){
				attron(COLOR_PAIR(10) | A_BOLD | A_BLINK);
			}
			mvaddch(j,i,map[j][i]);
			if (enemyMap[j][i] == '&'){
				attroff(A_BLINK);
				attron(COLOR_PAIR(23) | A_BOLD);
				mvaddch(j,i,'&');
			}
			attroff(A_BOLD | A_BLINK);
		}
	}
}

void drawFOV(int radius){
	int distX0 = 0;
	int distX1 = 0;
	int distY0 = 0;
	int distY1 = 0;
	init_pair(1, COLOR_WHITE , COLOR_BLACK);
	init_pair(2, COLOR_YELLOW , COLOR_BLACK);
	init_pair(3, COLOR_CYAN, COLOR_BLACK);
	init_pair(10, COLOR_RED, COLOR_BLACK);
	init_pair(23, COLOR_MAGENTA, COLOR_BLACK);
	clear();
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			if (seenMap[j][i] != ' '){
				attron(COLOR_PAIR(3));
				mvaddch(j,i,'#');
			} else {
				mvaddch(j,i,' ');
			}	
		}
	}
	if (playerEnt.currentPos.xPos < radius){
		distX0 = playerEnt.currentPos.xPos;
	} else if (WIDTH-playerEnt.currentPos.xPos < radius){
		distX1 = WIDTH-playerEnt.currentPos.xPos;
	}
	if (playerEnt.currentPos.yPos < radius){
		distY0 = playerEnt.currentPos.yPos;
	} else if (HEIGHT-playerEnt.currentPos.yPos < radius){
		distY1 = HEIGHT-playerEnt.currentPos.yPos;
	}
	for (int i = (playerEnt.currentPos.xPos+distX0)-radius ; i < (playerEnt.currentPos.xPos-distX1)+(radius+1) ; i++){
		for (int j = (playerEnt.currentPos.yPos+distY0)-radius ; j < (playerEnt.currentPos.yPos-distY1)+(radius+1) ; j++){
			if (j < HEIGHT && i < WIDTH){
				char currentChar = map[j][i];
				attron(COLOR_PAIR(1));
				if (currentChar == '.'){
					attron(COLOR_PAIR(2));
				} else if (currentChar == '+' || currentChar == '|' || currentChar == '-'){
					attron(COLOR_PAIR(1));	
				} else if (currentChar == '<'){
					attron(COLOR_PAIR(1));
				} else if (currentChar == '*'){
					attron(COLOR_PAIR(10) | A_BOLD | A_BLINK);
				}
				mvaddch(j,i,map[j][i]);
				if (enemyMap[j][i] == '&'){
					attroff(A_BLINK);
					attron(COLOR_PAIR(23) | A_BOLD);
					mvaddch(j,i,'&');
				}
				attroff(A_BOLD | A_BLINK);
				seenMap[j][i] = map[j][i];
			}
		}
	}
}

void drawDungeon(){
	for (int i = 0 ; i < WIDTH ; i++){
		for (int j = 0 ; j < HEIGHT ; j++){
			mvaddch(j,i,' ');
		}
	}
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	int seedMap = time(0);
	if (dungeonHasFire == 1){
		drawWithoutFOV();
	} else {
		drawFOV(3);
	}
	updateScreen();
}

void drawTown(){
	for (int i = 0 ; i < WIDTH ; i++){
		for (int j = 0 ; j < HEIGHT ; j++){
			mvaddch(j,i,' ');
		}
	}
	init_pair(1, COLOR_GREEN, COLOR_BLACK);
	init_pair(2, COLOR_YELLOW, COLOR_BLACK);
	init_pair(3, COLOR_WHITE, COLOR_BLACK);
	attroff(A_BOLD);
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			if (returnTownmapAt(i,j) == '.'){
				attron(COLOR_PAIR(1));
			} else if (returnTownmapAt(i,j) == '+'){
				attron(COLOR_PAIR(2));
			} else if (returnTownmapAt(i,j) == '#' || returnTownmapAt(i,j) == 'X'){
				attron(COLOR_PAIR(3));
			}
			mvaddch(j,i,returnTownmapAt(i,j));
		}
	}
	updateScreen();
}

void drawStarmap(){
	init_pair(1, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(1));
	attron(A_BOLD);
	for (int i = 0 ; i < 79 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			mvaddch(j, i, skyMap[j][i]);
		}
	}
	attroff(A_BOLD);
}

void drawEnemies(){
	init_pair(23, COLOR_MAGENTA, COLOR_BLACK);
	if (biome == 'd'){
		for (int i = 0 ; i < 80 ; i++){
			for (int j = 0 ; j < 24 ; j++){
				if (enemyMap[j][i] == '&'){
					attron(COLOR_PAIR(23) | A_BOLD);
					mvaddch(j,i,'&');
				}
			}
		}
	}
	attroff(A_BOLD);
}

char charAtCoord(int x, int y, char wantedChar){
	return (mvwinch(stdscr, y, x) & A_CHARTEXT)-wantedChar;
}
