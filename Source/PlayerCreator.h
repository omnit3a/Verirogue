#ifndef PLAYERCREATOR_H_
#define PLAYERCREATOR_H_
#include <string>

extern int disabledCreator;

extern std::string defaultName;
extern char defaultSex;

char getPlayerSex();

char getStarSign();

std::string getPlayerName();

void playerCreatorScreen();

#endif
