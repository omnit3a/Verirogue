#include <stdio.h>
#include <stdlib.h>
#include "DrawUI.h"
#include <ncurses.h>
#include "Player.h"
#include "Movement.h"
#include "MapGenerator.h"
#include "Main.h"
#include <string>

std::string msgLog = "Welcome to Verirogue";

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
	attron(COLOR_PAIR(8));
	mvprintw(2,81,"Turn: %d",turn);
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
	mvprintw(1,81,"%s",msgLog.c_str());
	attroff(A_REVERSE);
	refresh();
}

void drawHealth(){
	init_pair(8, COLOR_WHITE, COLOR_BLACK);
	attroff(A_BOLD);
	attron(COLOR_PAIR(8));
	int headHP = playerEnt.head.bpHP.currentHealth;
	int torsoHP = playerEnt.torso.bpHP.currentHealth;
	int lArmHP = playerEnt.leftArm.bpHP.currentHealth;
	int rArmHP = playerEnt.rightArm.bpHP.currentHealth;
	int lLegHP = playerEnt.leftLeg.bpHP.currentHealth;
	int rLegHP = playerEnt.rightLeg.bpHP.currentHealth;
	if (headHP < 25) { attron(A_BLINK); }
	mvprintw(3,81,"Head: %d",headHP);
	attroff(A_BLINK);
	if (torsoHP < 25) { attron(A_BLINK); }
	mvprintw(4,81,"Torso: %d",torsoHP);
	attroff(A_BLINK);
	if (lArmHP < 25) { attron(A_BLINK); }
       	mvprintw(5,81,"Left Arm: %d",lArmHP);
	attroff(A_BLINK);
	if (rArmHP < 25) { attron(A_BLINK); }
	mvprintw(6,81,"Right Arm: %d",rArmHP);
	attroff(A_BLINK);
	if (lLegHP < 25) { attron(A_BLINK); }
	mvprintw(7,81,"Left Leg: %d",lLegHP);
	attroff(A_BLINK);
	if (rLegHP < 25) { attron(A_BLINK); }
	mvprintw(8,81,"Right Leg: %d",rLegHP);
	attroff(A_BLINK);
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
	mvprintw(27,0,"                                                   ");
	mvprintw(27,0,"Hydration: %d", playerEnt.currentHydration.hydration);
	attroff(A_BLINK | A_BOLD);
	refresh();
}

void drawHunger(){
	init_pair(11, COLOR_WHITE, COLOR_BLACK);
	init_pair(12, COLOR_RED, COLOR_BLACK);
	attron(A_BOLD);
	if (foodScore < 50){
		attron(COLOR_PAIR(12) | A_BLINK);
	} else {
		attron(COLOR_PAIR(11));
	}
	mvprintw(28,0,"                          ");
	mvprintw(28,0,"Nourishment: %d", foodScore);
	attroff(A_BLINK | A_BOLD);
	refresh();
}

void drawGold(){
	init_pair(11, COLOR_WHITE, COLOR_BLACK);
	init_pair(17, COLOR_YELLOW, COLOR_BLACK);
	attron(COLOR_PAIR(11) | A_BOLD);
	mvprintw(29,0,"                     ");
	mvprintw(29,0,"Gold: ");
	attron(COLOR_PAIR(17));
	mvprintw(29,6,"%d",goldScore);
	attroff(A_BOLD);
	refresh();
}

void drawSheet(){
	init_pair(11, COLOR_WHITE, COLOR_BLACK);
	clear();
	attron(COLOR_PAIR(11) | A_BOLD);
	mvprintw(0,0,"### Unnamed hero ###");
	mvprintw(1,0,"Sex: %c",playerEnt.currentSex.sex);
	mvprintw(2,0,"Weight: %d",playerEnt.currentWeight.pounds);
	mvprintw(3,0,"# ---------------- #");
	mvprintw(4,0,"Gold: %d", goldScore);
	attroff(A_BOLD);
	refresh();
	getch();
}

void drawUserInterface(){
	drawTemperature();
	drawLog();
	drawTurn();
	drawHealth();
	drawHydration();
	drawHunger();
	drawGold();
	refresh();
}
