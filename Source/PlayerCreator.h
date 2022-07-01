#define HUMANSYM '@'
#define GOBLINSYM '&'
#define KOBOLDSYM 'k'

#ifndef PLAYERCREATOR_H_
#define PLAYERCREATOR_H_
#include <string>

extern int disabledCreator;

extern std::string defaultName;
extern char defaultSex;
extern std::string defaultRace;

char getPlayerSex();

char getStarSign();

std::string getPlayerName();

std::string getPlayerRace();

void playerCreatorScreen();

#endif
