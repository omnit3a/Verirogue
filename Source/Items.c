#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include <time.h>
#include "Player.h"
#include "Movement.h"
#include "Inventory.h"
#include "DrawUI.h"

void quaffPotion(){
	if (hasPotion()){
		consumePotion();
		msgLog = "You drank a potion";
		srand(time(0)+itemCount);
		previousWater = playerEnt.currentHydration.hydration;
		playerEnt.currentHydration.hydration += 5;
		int effect = rand() % 1000;
		if (effect <= 250){
			msgLog = "Your appendages are restored!";
			playerEnt.leftLeg.bpHP.currentHealth = 100;
			playerEnt.rightLeg.bpHP.currentHealth = 100;
			playerEnt.leftArm.bpHP.currentHealth = 100;
			playerEnt.rightArm.bpHP.currentHealth = 100;
		} else if (effect > 250 && effect <= 300){
			msgLog = "Your head is affected";
			playerEnt.head.bpHP.currentHealth = 50;
		} else if (effect > 300 && effect <= 400){
			msgLog = "You are maximally hydrated!";
			playerEnt.currentHydration.hydration = 75;
		} else if (effect > 400 && effect <= 600){
			msgLog = "You are granted 25 gold!";
			goldScore += 25;
		} else if (effect > 600 && effect <= 750){
			msgLog = "Your legs are affected";
			playerEnt.leftLeg.bpHP.currentHealth = 35;
			playerEnt.rightLeg.bpHP.currentHealth = 35;
		} else if (effect > 750 && effect <= 800){
			msgLog = "Your arms are affected";
			playerEnt.leftArm.bpHP.currentHealth = 35;
			playerEnt.rightArm.bpHP.currentHealth = 35;
		} else {
			msgLog = "Nothing happens!";
		}

	} else {
		msgLog = "You don't have a potion";
	}
}

void readScroll(){
	if (hasScroll() && playerEnt.currentStats.intelligence >= 50){
		consumeScroll();
		msgLog = "You read a scroll";
		srand(time(0)+itemCount);
		//insert code for scroll reading here
	} else if (playerEnt.currentStats.intelligence < 50){
		consumeScroll();
		msgLog = "You aren't able to read the scroll";
	} else {
		msgLog = "You don't have a scroll";
	}
}
