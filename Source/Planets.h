#ifndef PLANETS_H_
#define PLANETS_H_

extern char skyMap[][80];

extern int month, day, hour;

/**
 *	Index 0 is the current month
 *	Index 1 is the current day in the month
 */
extern int calender[][30];

extern int isDay;

extern int sunX, sunY;

/**
 *	Generate map of the sky, this includes stars and planets
 *
 */
void generatePlanetaryBodies(int maxPlanets, int maxStars);

/**
 *	Make the stars move through the sky
 *
 */
void scrollStars();

/**
 *	Set the time of day depending on the current position of the sun
 *	
 */
void setDayNight();
#endif
