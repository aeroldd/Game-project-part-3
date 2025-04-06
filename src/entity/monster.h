#ifndef MONSTER_H
#define MONSTER_H


// Runs the monster AI
void runMonster(RoomGrid *room, Entity *entity, Entity *player);

// Move the monster randomly in the room when the player isnt in attack range
void monsterRoam(RoomGrid *room, Entity *entity, int maxDistance);

#endif