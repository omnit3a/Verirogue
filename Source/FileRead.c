#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Player.h"
#include "DrawScreen.h"
#include <ncurses.h>

void initFileRead(){
	FILE *ptr;
	char str[50];
	ptr = fopen("../Init/init.txt", "r");

	if (NULL==ptr){
		killPlayer("Unable to read init file\n");
	}
	int index = 0;
	while (fgets(str, 50, ptr) != NULL){
		switch(index){
			case 0:
				if (str[0] == 'Y'){
					grossStuff = 1;
				} else {
					grossStuff = 0;
				}	
				break;
			case 1:
				if (str[0] == 'Y'){
					noFov = 1;
				} else {
					noFov = 0;
				}
				break;
			default:
				continue;
		}
		index++;
	}

	fclose(ptr);
}
