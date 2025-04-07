#ifndef ENTITY_H
#define ENTITY_H

#include "../core/game.h"

/*
ALL CODE PERTAINING TO THE CREATION OF ENTITIES
previously this file contained all entity related functions but it made it really unmodular
*/

// ENTITY CREATION

extern int id;

Entity *createEntity(char *name, int type, char symbol, Position mapPos, Position gridPos,
                       int maxHP, int ac, int speed, int initiativeMod, int attack, int damage, int gold,
                       int viewRadius, int blind);
Entity *createEntityFromFile(char* entityName, Position mapPos, Position gridPos, int currentHealth);
void setEntityName(Entity *entity, const char* name);


void createPlayer(Entity **player);

Entity *getEntityAtGridPosition(RoomGrid *room, Position pos);
int setEntityPosition(RoomGrid *room, Entity *e, Position pos);

Entity **createEntitiesArray();

// Entity array functions
int getEntityCount(Entity **entities);
void addEntityToArray(Entity ***entitiesList, int *count, Entity *entity);
void removeEntityById(Entity ***entities, int *count, int targetId);
Entity *getEntityById(Entity **entities, int entityCount, int entityId);

void printEntityStats(Entity *e);
void printEntities(Entity **e, int entityCount);

#endif