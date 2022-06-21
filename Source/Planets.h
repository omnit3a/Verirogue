#ifndef PLANETS_H_
#define PLANETS_H_

extern char skyMap[][80];

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
