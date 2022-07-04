#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "Player.h"
#include "Inventory.h"
#include "MapGenerator.h"
#include "Movement.h"
#include "Legacy.h"
#include <string>
#include <cstring>

char itemMap[24][80];

int itemCount = 0;

std::string inventory[17];

int itemTurn[17];

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
	} else if (text == "Berries"){
		return BERRYSYM;
	} else if (text == "Wintergreen"){
		return WINTERSYM;
	} else if (text == "Shellfish"){
		return SHELLFISHSYM;
	} else if (text == "Rotten food"){
		return ROTTENSYM;
	} else {
		return ' ';
	}
}

void addItem(char * item, int index){
	inventory[index] = convertToString(item);
	itemTurn[index] = turn;
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
		itemsLost++;
	}
	inventory[index] = "";
}

int hasFood(){ 
	if (itemCount-1 != -1 && (inventory[itemCount-1] == "Berries" || inventory[itemCount-1] == "Shellfish" || inventory[itemCount-1] == "Wintergreen")){
		return 1;
	}
	return 0;
}

void consumeFood(){
	if (representation(inventory[itemCount]) == ' ' && itemCount-1 != -1){
		itemCount--;
	}
	if (inventory[itemCount] == "Berries" || inventory[itemCount] == "Shellfish" || inventory[itemCount] == "Wintergreen"){
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
			if (biome == 'd' && dungeonWalkable(i,j)){
				if (rand() % 228 == 0){
					int item = rand() % 3;
					if (item == 0){
						itemMap[j][i] = SCROLLSYM;
					} else if (item == 1){
						itemMap[j][i] = POTIONSYM;
					}
				}
			}
		}
	}
}

void rotItems(){
	for (int i = 0 ; i < 16 ; i++){
		if (inventory[i] == "Berries" || inventory[i] == "Shellfish" || inventory[i] == "Wintergreen"){
			itemTurn[i]++;
			if(itemTurn[i] == 400){
				inventory[i] = "Rotten food";
				itemTurn[i] = 0;
			}
		}
	}
}
