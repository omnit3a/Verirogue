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
		playerEnt.currentHydration.hydration += 5;
		int effect = rand() % 1000;
		// insert code for potion effects here
		if (effect <= 250){
			msgLog = "Your appendages are restored!";
			playerEnt.leftLeg.bpHP.currentHealth = 100;
			playerEnt.rightLeg.bpHP.currentHealth = 100;
			playerEnt.leftArm.bpHP.currentHealth = 100;
			playerEnt.rightArm.bpHP.currentHealth = 100;
		} else if (effect > 250 && effect <= 300){
			msgLog = "You now have a headache!";
			playerEnt.head.bpHP.currentHealth = 50;
		} else if (effect > 300 && effect <= 400){
			msgLog = "You are maximally hydrated!";
			playerEnt.currentHydration.hydration = 75;
		} else if (effect > 400 && effect <= 600){
			msgLog = "You are granted 25 gold!";
			goldScore += 25;
		} else if (effect > 600 && effect <= 750){
			msgLog = "Your legs vanish!";
			playerEnt.leftLeg.bpHP.currentHealth = 0;
			playerEnt.rightLeg.bpHP.currentHealth = 0;
		} else if (effect > 750 && effect <= 800){
			msgLog = "Your arms vanish!";
			playerEnt.leftArm.bpHP.currentHealth = 0;
			playerEnt.rightArm.bpHP.currentHealth = 0;
		} else {
			msgLog = "Nothing happens!";
		}

	}
}
