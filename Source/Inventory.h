#ifndef INVENTORY_H_
#define INVENTORY_H_

#include <string>

extern std::string inventory[16];

std::string convertToString(char * text);

char * convertToChars(std::string text);

void addItem(char * item);

#endif

