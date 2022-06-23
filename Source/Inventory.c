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

char representation(std::string text){
	if (text == "Sword"){
		return SWORDSYM;
	} else if (text == "Axe"){
		return AXESYM;
	} else if (text == "Scroll"){
		return SCROLLSYM;
	} else if (text == "Potion"){
		return POTIONSYM;
	} else if (text == "Food"){
		return FOODSYM;
	} else {
		return ' ';
	}
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

void dropItem(int index){
	if (biome == 'd'){
		itemMap[playerEnt.currentPos.yPos][playerEnt.currentPos.xPos] = representation(inventory[index]);
	}
	inventory[index] = "";
}

int hasFood(){
	if (itemCount-1 != -1 && inventory[itemCount-1] == "Food"){
		return 1;
	}
	return 0;
}

void consumeFood(){
	if (representation(inventory[itemCount]) == ' ' && itemCount-1 != -1){
		itemCount--;
	}
	if (inventory[itemCount] == "Food"){
		inventory[itemCount] = "";

	}
}

int hasPotion(){
	if (itemCount-1 != -1 && inventory[itemCount-1] == "Potion"){
		return 1;
	}
	return 0;
}

void consumePotion(){
	if (representation(inventory[itemCount]) == ' ' && itemCount-1 != -1){
		itemCount--;
	}
	if (inventory[itemCount] == "Potion"){
		inventory[itemCount] = "";
	}
}

int hasScroll(){
	if (itemCount-1 != -1 && inventory[itemCount-1] == "Scroll"){
		return 1;
	}
	return 0;
}

void consumeScroll(){
	if (representation(inventory[itemCount]) == ' ' && itemCount-1 != -1){
		itemCount--;
	}
	if (inventory[itemCount] == "Scroll"){
		inventory[itemCount] = "";
	}
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
					int item = rand() % 3;
					if (item == 0){
						itemMap[j][i] = SCROLLSYM;
					} else if (item == 1){
						itemMap[j][i] = POTIONSYM;
					} else if (item == 2){
						itemMap[j][i] = FOODSYM;
					}
				}
			}
		}
	}
}
