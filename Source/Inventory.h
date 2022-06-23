#define SWORDSYM '!'
#define AXESYM 'A'
#define SCROLLSYM 'S'

#ifndef INVENTORY_H_
#define INVENTORY_H_

#include <string>

extern char itemMap[][80];

extern std::string inventory[16];

std::string convertToString(char * text);

char * convertToChars(std::string text);

void addItem(char * item);

char * getItem(int index);

char itemAt(int x, int y);

void placeItems();
#endif

