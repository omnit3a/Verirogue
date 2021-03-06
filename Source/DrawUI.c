#include <stdio.h>
#include <stdlib.h>
#include "DrawUI.h"
#include <ncurses.h>
#include "Player.h"
#include "Movement.h"
#include "MapGenerator.h"
#include "Planets.h"
#include "Main.h"
#include <string>
#include "Inventory.h"

std::string msgLog = "Welcome to Verirogue";
int previousTemperature;
int previousNourishment;
int previousGold;
int previousBlood;
int previousWater;

void drawTemperature(){
	if (playerEnt.currentTemperature.celsius != previousTemperature){
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
	init_pair(22, COLOR_BLACK, COLOR_YELLOW);
	init_pair(21, COLOR_BLACK, COLOR_BLUE);
	attron(COLOR_PAIR(9) | A_BOLD);
	for (int i = 0 ; i < 120 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			if (( i == 0 ||i == 80 || i == 119) || (j == 0 || j == 23)){
				attron(A_BOLD);		//draw border of log
				attroff(A_REVERSE);
				mvaddch(j,i,' ');
			} else {
				if (i > 80){
					attroff(A_BOLD);	//draw inside of log
					attron(A_REVERSE);
					mvaddch(j,i,' ');
				}
			}
		}
	}
	attroff(A_BOLD);
	attron(A_REVERSE);
	mvprintw(0,2," Verirogue ");
	mvprintw(1,81,"%s",msgLog.c_str());
	mvprintw(9,81,"                                     ");
	if (isDay == 1){
		attron(COLOR_PAIR(22) | A_BOLD);
		mvprintw(9,81,"It is the daytime.");
	} else {
		attron(COLOR_PAIR(21) | A_BOLD);
		mvprintw(9,81,"It is the nighttime");
	}
	mvprintw(10,81,"                               ");
	mvprintw(10,81,"Month %d, Day %d, Hour %d",month,day,hour);
	attroff(A_REVERSE | A_BOLD);
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
}	

void drawHydration(){
	if (playerEnt.currentHydration.hydration != previousWater){
		init_pair(11, COLOR_WHITE, COLOR_BLACK);
		init_pair(12, COLOR_RED, COLOR_BLACK);
		attron(A_BOLD);
		if (playerEnt.currentHydration.hydration <= 40){
			attron(COLOR_PAIR(12) | A_BLINK);
		} else if (playerEnt.currentHydration.hydration > 40){
			attron(COLOR_PAIR(11));
		}
		mvprintw(27,0,"                                                   ");
		mvprintw(27,0,"Hydration: %d", playerEnt.currentHydration.hydration);
		attroff(A_BLINK | A_BOLD);
	}
}

void drawHunger(){
	if (foodScore != previousNourishment){
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
	}
}

void drawGold(){
	if (goldScore != previousGold){
		init_pair(11, COLOR_WHITE, COLOR_BLACK);
		init_pair(17, COLOR_YELLOW, COLOR_BLACK);
		attron(COLOR_PAIR(11) | A_BOLD);
		mvprintw(29,0,"                     ");
		mvprintw(29,0,"Gold: ");
		attron(COLOR_PAIR(17));
		mvprintw(29,6,"%d",goldScore);
		attroff(A_BOLD);
	}
}

void drawSheet(){
	init_pair(11, COLOR_WHITE, COLOR_BLACK);
	clear();
	attron(COLOR_PAIR(11) | A_BOLD);
	mvprintw(0,0,"Name: %s",playerName.c_str());
	mvprintw(1,0,"Sex: %c",playerEnt.currentSex.sex);
	mvprintw(2,0,"Race: %s",playerRace.c_str());
	mvprintw(3,0,"Weight: %d",playerEnt.currentWeight.pounds);
	mvprintw(4,0,"Star Sign: %c",playerEnt.currentSign.sign);
	mvprintw(5,0,"Strength: %d",playerEnt.currentStats.strength);
	mvprintw(6,0,"Agility: %d",playerEnt.currentStats.agility);
	mvprintw(7,0,"Intelligence: %d",playerEnt.currentStats.intelligence);
	mvprintw(8,0,"Handedness: %s",playerHandedness.c_str());
	mvprintw(9,0,"# ---------------- #");
	mvprintw(10,0,"Gold: %d", goldScore);
	attroff(A_BOLD);
	refresh();
	getch();
}

void drawHelp(){
	init_pair(11, COLOR_WHITE, COLOR_BLACK);
	clear();
	attron(COLOR_PAIR(11) | A_BOLD);
	mvprintw(0,0,"Arrow keys : Movement");
	mvprintw(1,0,"> : Enter tile");
	mvprintw(2,0,"< : Exit tile");
	mvprintw(3,0,"f : Set fire");
	mvprintw(4,0,"s : Scavenge for food");
	mvprintw(5,0,"S : Check character sheet");
	mvprintw(6,0,"e : Eat food");
	mvprintw(7,0,"m : View sky");
	mvprintw(8,0,"M : Medical");
	mvprintw(9,0,"  c : Cauterize");
	mvprintw(10,0,"  C : Clean floor");
	mvprintw(11,0,"  d : Cure disease");
	mvprintw(12,0,"p : Go prone/Start swimming");
	mvprintw(13,0,"q : Drink from lake");
	mvprintw(14,0,"Q : Drink potion");
	mvprintw(15,0,"i : View inventory");
	mvprintw(16,0,", : Pickup item");
	mvprintw(17,0,"d : Drop item");
	mvprintw(18,0,"D : Describe");
	mvprintw(19,0,"r : Read scroll");
	mvprintw(20,0,"x : Swap hand");
	mvprintw(21,0,"? : View this screen");
	attroff(A_BOLD);
	refresh();
	getch();
	
}

void drawInventory(){
	init_pair(11, COLOR_WHITE, COLOR_BLACK);
	clear();
	attron(COLOR_PAIR(11) | A_BOLD);
	mvprintw(0,0,"Inventory:");
	for (int i = 0 ; i < 16 ; i++){
		mvprintw(i+1,0," - %s",inventory[i].c_str());
	}
	attroff(A_BOLD);
	refresh();
	getch();
}

void drawDropScreen(){
	init_pair(11, COLOR_WHITE, COLOR_BLACK);
	clear();
	attron(COLOR_PAIR(11) | A_BOLD);
	if (representation(inventory[itemCount]) == ' ' && itemCount-1 != -1){
		itemCount--;
	}
	if (representation(inventory[itemCount]) != ' '){
		mvprintw(0,0,"Drop the top item in your bag?");
		mvprintw(1,0,"Y/N");
		for (int i = 0 ; i < 16 ; i++){
			mvprintw(i+2,0," - %s ",inventory[i].c_str());
		}
		attroff(A_BOLD);
		int confirm = getch();
		if (confirm == 'Y' || confirm == 'y'){
			dropItem(itemCount);
			itemCount--;
		}
		if (itemCount < 0){
			itemCount++;
		}
	} else {
		mvprintw(0,0,"You don't have any items to drop");
		getch();
	}
	refresh();
}

void drawBlood(){
	if (bloodCount != previousBlood){
		init_pair(11,COLOR_WHITE,COLOR_BLACK);
		init_pair(28,COLOR_RED,COLOR_BLACK);
		attroff(A_BLINK);
		if (bloodCount < 384){
			attron(COLOR_PAIR(28) | A_BOLD | A_BLINK);
		} else {
			attron(COLOR_PAIR(11) | A_BOLD);
		}
		mvprintw(30,0,"Blood: %d",bloodCount);
		attroff(A_BOLD | A_BLINK);
	}
}

void drawIsInfected(){
	init_pair(29,COLOR_GREEN,COLOR_BLACK);
	init_pair(28,COLOR_RED,COLOR_BLACK);
	attroff(A_BLINK);
	if (isDiseased){
		attron(COLOR_PAIR(28) | A_BOLD);
		mvprintw(11,81,"You are diseased!");
	}
	if (isInfected){
		attron(COLOR_PAIR(28) | A_BOLD);
		mvprintw(11,81,"You are infected!");
	}
	if (!isDiseased && !isInfected){
		attron(COLOR_PAIR(29) | A_BOLD);
		mvprintw(11,81,"You are healthy  ");
	}
	attroff(A_BOLD);

}

void drawUserInterface(){
	drawTemperature();
	drawLog();
	drawTurn();
	drawHealth();
	drawHydration();
	drawHunger();
	drawGold();
	drawBlood();
	drawIsInfected();
	refresh();
}
