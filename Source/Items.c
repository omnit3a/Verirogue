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

	}
}
