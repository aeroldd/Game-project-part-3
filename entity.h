#ifndef ENTITY_H
#define ENTITY_H

#include "game.h"

extern int id;

Entity *createEntity(char *name, int type, char symbol, Position mapPos, Position gridPos,
                       int maxHP, int ac, int speed, int initiativeMod, int attack, int damage, int gold);
Entity *createEntityFromFile(char* entityName, Position mapPos, Position gridPos, int currentHealth);

void createPlayer(Entity **player);
Entity *createPlayerEntity(char *name, char symbol, Position mapPos, Position gridPos);
Entity *getEntityAtGridPosition(RoomGrid *room, Position pos);
int setEntityPosition(RoomGrid *room, Entity *e, Position pos);
Entity **createEntitiesArray();
int getEntityCount(Entity **entities);
void addEntityToArray(Entity ***entitiesList, int *count, Entity *entity);
void removeEntityById(Entity ***entities, int *count, int targetId);
Entity *getEntityById(Entity **entities, int entityCount, int entityId);
void printEntityStats(Entity *e);
void printEntities(Entity **e, int entityCount);
void runEntity(RoomGrid *room, Entity *entity, Entity *player);
void runMonster(RoomGrid *room, Entity *entity, Entity *player);
void monsterRoam(RoomGrid *room, Entity *entity, int maxDistance);


#endif
