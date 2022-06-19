#ifndef DRAWSCREEN_H_
#define DRAWSCREEN_H_

extern int row, col;

void setupScreen();

void updateScreen();

void endScreen();

void drawMap();

void drawCountries();

extern char seenMap[24][80];

void drawWithoutFOV();

void drawFOV(int radius);

void drawDungeon();

char charAtCoord(int x, int y, char wantedChar);

#endif

