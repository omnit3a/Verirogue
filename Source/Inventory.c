#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "Player.h"
#include "Inventory.h"
#include "MapGenerator.h"
#include "Movement.h"
#include <string>
#include <cstring>

char itemMap[24][80];

int itemCount = 0;

std::string inventory[16];

std::string convertToString(char * text){
	int length = sizeof(text) / sizeof(char);
	std::string s = "";
	for (int i = 0 ; i < length ; i++){
		s = s + text[i];
	}
	return s;
}

char * convertToChars(std::string text){
	char arr[text.length() + 1];
	return strcpy(arr, text.c_str());
}

void addItem(char * item, int index){
	inventory[index] = convertToString(item);
}

void removeItem(int index){
	if (index >= 0){
		inventory[index] = "";
	}
}

char * getItem(int index){
	return convertToChars(inventory[index]);
}

char itemAt(int x, int y){
	return itemMap[y][x];
}

void placeItems(){
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			itemMap[j][i] = ' ';
		}
	}
	srand(time(0));
	for (int i = 0 ; i < 80 ; i++){
		for (int j = 0 ; j < 24 ; j++){
			if (biome == 'd' && map[j][i] != ' '){
				if (rand() % 228 == 0){
					if (rand() % 2 == 0){
						itemMap[j][i] = SCROLLSYM;
					} else {
						itemMap[j][i] = POTIONSYM;
					}
				}
			}
		}
	}
}
