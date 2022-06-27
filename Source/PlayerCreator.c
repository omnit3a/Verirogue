#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>
#include "Player.h"
#include "PlayerCreator.h"
#include "DrawScreen.h"
#include <string>

int disabledCreator;

std::string defaultName;

char defaultSex;

char getPlayerSex(){
	attron(A_BLINK);
	mvaddch(1,0,'>');
	attroff(A_BLINK);
	updateScreen();
	char ch;
	getSex:
		ch = getch();
	if((ch == 'm' || ch == 'M') || (ch == 'f' || ch == 'F') || (ch == 'u' || ch == 'U')){
		return ch;
	} else {
		goto getSex;
	}
}

char getStarSign(){
	srand(time(0));
	int sign = (rand() % 3)+1;
	return (char)sign+64;
}

std::string getPlayerName(){
	int ch;
	char name[1024];
	attron(A_BLINK);
	mvaddch(1,0,'>');
	attroff(A_BLINK);
	updateScreen();
	getstr(name);
	return name;
 }

void playerCreatorScreen(){
	if (!disabledCreator){
		mvprintw(0,0,"You have crashed through the universe onto a flat plane of earth.");
		updateScreen();
		getch();
		clear();
		mvprintw(0,0,"What is your sex? M | F | U");
		updateScreen();
		playerEnt.currentSex.sex = getPlayerSex();
		playerEnt.currentSign.sign = getStarSign();
		clear();
		mvprintw(0,0,"What is your name?");
		updateScreen();
		playerName = getPlayerName();
	} else {
		playerName = defaultName;
		playerEnt.currentSex.sex = defaultSex;
	}
}
