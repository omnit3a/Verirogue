#include <stdio.h>
#include <stdlib.h>
#include "DrawUI.h"
#include <ncurses.h>
#include "Player.h"
#include "Movement.h"
#include "MapGenerator.h"
#include "Main.h"

char * msgLog = "Welcome to Verirogue";

void drawTemperature(){
	init_pair(13, COLOR_CYAN, COLOR_BLACK);
	init_pair(8, COLOR_WHITE, COLOR_BLACK);
	init_pair(14, COLOR_RED, COLOR_BLACK);
	attron(A_BOLD);
	if (playerEnt.currentTemperature.celsius < 35){
		attron(COLOR_PAIR(13));
	} else if (playerEnt.currentTemperature.celsius > 39){
		attron(COLOR_PAIR(14));
	} else {
		attron(COLOR_PAIR(8));
	}
	mvprintw(25,0,"                    ");
	mvprintw(26,0,"                    ");
	mvprintw(25,0,"Temperature: %d^C",playerEnt.currentTemperature.celsius);
	mvprintw(26,0,"           : %d^F",playerEnt.currentTemperature.fahrenheit);
	attroff(A_BOLD);
}

void drawTurn(){
	init_pair(8, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(8) | A_BOLD);
	mvprintw(27,0,"Turn: %d",turn);
	attroff(A_BOLD);
}

void drawSeeds(){
	init_pair(8, COLOR_WHITE, COLOR_BLACK);
	attron(COLOR_PAIR(8) | A_BOLD);
	mvprintw(28,0,"Seeds: %d fromPosition",seedFromPosition(entryX, entryY));
	mvprintw(29,0,"     : %d overworld",seedMain);
	attroff(A_BOLD);
}

void drawLog(){
	init_pair(9, COLOR_BLACK, COLOR_WHITE);
	attron(COLOR_PAIR(9) | A_BOLD);
	for (int i = 80 ; i < 120 ; i++){
		for (int j = 0 ; j < 12 ; j++){
			mvaddch(j,i,' ');
		}
	}
	attroff(A_BOLD);
	attron(A_REVERSE);
	for (int i = 81 ; i < 119 ; i++){
		for (int j = 1 ; j < 11 ; j++){
			mvaddch(j,i,' ');
		}
	}
	mvprintw(1,81,"%s",msgLog);
	attroff(A_REVERSE);
	refresh();
}

void drawHydration(){
	init_pair(11, COLOR_WHITE, COLOR_BLACK);
	init_pair(12, COLOR_RED, COLOR_BLACK);
	attron(A_BOLD);
	if (playerEnt.currentHydration.hydration <= 55){
		attron(COLOR_PAIR(12) | A_BLINK);
	} else if (playerEnt.currentHydration.hydration > 55){
		attron(COLOR_PAIR(11));
	}
	mvprintw(30,0,"Hydration: %d%", playerEnt.currentHydration.hydration);
	attroff(A_BLINK | A_BOLD);
	refresh();
}

void drawUserInterface(){
	drawTemperature();
	drawTurn();
	drawSeeds();
	drawLog();
	drawHydration();
	refresh();
}
