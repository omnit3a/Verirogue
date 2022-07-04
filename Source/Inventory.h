#define SWORDSYM '!'
#define AXESYM 'A'
#define SCROLLSYM 'S'
#define POTIONSYM 'p'
#define BERRYSYM '%'
#define WINTERSYM 'W'
#define SHELLFISHSYM 'c'
#define ROTTENSYM ';'

#ifndef INVENTORY_H_
#define INVENTORY_H_

#include <string>

extern char itemMap[][80];

extern int itemCount;

extern std::string inventory[17];

extern int itemTurn[17];

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

int hasScroll();

void consumeScroll();

char itemAt(int x, int y);

void placeItems();

void rotItems();
#endif

