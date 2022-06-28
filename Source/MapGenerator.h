#ifndef MAPGENERATOR_H_
#define MAPGENERATOR_H_

extern int dungeonHasFire;

extern int coastmap[][80];

void generateCoastmap();

/**
 *	generate the map of the coast
 */
int returnCoastmapAt(int x, int y);

extern char map[][80];

extern char surroundingChar[4];

extern char underPlayer;

extern int treeSpawnRate,dungeonSpawnRate,canFireSpread;

void generateMap();

/**
 *	generate the heightmap
 */
char returnHeightmapAt(int x, int y);

char returnCountryAt(int x, int y);

extern int startX, startY;

void generateEngraving(int x, int y);

void generateWall();

void generateDungeon(int maxWidth, int maxHeight);

char returnDungeonmapAt(int x, int y);

extern int townMap[][80];

extern int townNPCS[][4];

void generateTown(int maxWidth, int maxHeight);

char returnTownmapAt(int x, int y);

int seedFromPosition(int x, int y);

extern char fireValues[][80];

void fireSpread();

#endif
