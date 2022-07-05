#ifndef NPC_H_
#define NPC_H_

#define MAX_NPCS 10

extern char npcMap[][80];
extern int npcCount;
extern int npcPos[][2];

void placeNPCS(int amount);

void pathfindNPCS();

#endif
