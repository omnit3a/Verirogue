#ifndef HISTORY_H_
#define HISTORY_H_
#include <string>

extern std::string civilizations[4];
extern int civilizationPeaceful[4];
extern int civilizationRacist[4];
extern int civilizationSmart[4];

std::string returnGoblinName(int pairs);

void generateCivilizations();

#endif
