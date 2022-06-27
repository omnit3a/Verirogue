#define SMOKESYM '*'
#define STEAMSYM '"'

#ifndef GAS_H_
#define GAS_H_

/**
 *	Contains the type of gas on a tile
 */
extern char gasMap[][80];

/**
 *	Contains the density of gases
 */
extern int gasValues[][80];

void initGas();

void placeGas(int x, int y, char gasType);

void diffuseGas(int x, int y);

#endif
