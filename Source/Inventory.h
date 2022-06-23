#define SWORDSYM '!'
#define AXESYM 'A'
#define SCROLLSYM 'S'
#define POTIONSYM 'p'
#define FOODSYM '%'

#ifndef INVENTORY_H_
#define INVENTORY_H_

#include <string>

extern char itemMap[][80];

extern int itemCount;

extern std::string inventory[16];

std::string convertToString(char * text);

char * convertToChars(std::string text);

char representation(std::string text);

void addItem(char * item);

char * getItem(int index);

void dropItem(int index);

int hasFood();

void consumeFood();

int hasPotion();

void consumePotion();

char itemAt(int x, int y);

void placeItems();
#endif

