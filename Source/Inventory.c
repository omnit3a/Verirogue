#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>
#include "Player.h"
#include "Inventory.h"
#include <string>
#include <cstring>

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
