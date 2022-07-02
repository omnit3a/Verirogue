#ifndef HISTORY_H_
#define HISTORY_H_
#include <string>

extern std::string civilizations[4];
extern std::string leaders[4];
extern int civilizationStyle[4];
extern int civilizationPeaceful[4];
extern int civilizationRacist[4];
extern int civilizationSmart[4];
extern int civilizationWars[][4];

extern std::string worldHistory;

std::string returnGoblinName(int pairs);

void generateCivilizations();

void generateWarData(int civNumber);

std::string generateCivData();

#endif
