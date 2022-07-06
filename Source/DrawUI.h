#include <string>

#ifndef DRAWUI_H_
#define DRAWUI_H_

extern std::string msgLog;

/**
 *	These keep track of the previous values of stats that are drawn on screen.
 *	The stats that are on screen are now only updated when the value changes.
 */
extern int previousTemperature;
extern int previousNourishment;
extern int previousGold;
extern int previousBlood;
extern int previousWater;

void drawTemperature();

void drawTurn();

void drawSeed();

void drawLog();

void drawHealth();

void drawHydration();

void drawHunger();

void drawGold();

void drawSheet();

void drawHelp();

void drawInventory();

void drawDropScreen();

void drawBlood();

void drawIsInfected();

void drawUserInterface();

#endif
