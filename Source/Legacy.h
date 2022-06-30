#ifndef LEGACY_H_
#define LEGACY_H_
#include <string>

extern int enemiesSlain;
extern int goldFound;
extern int bloodLost;
extern int foodEaten;
extern int gluttonCount;
extern int potionsQuaffed;
extern int itemsFound;
extern int itemsLost;
extern int medicalSuccesses;
extern int medicalFailures;

std::string generateLegacy();

void saveLegacy();

#endif
