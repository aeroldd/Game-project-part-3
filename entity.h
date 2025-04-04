#ifndef ENTITY_H
#define ENTITY_H

#include "game.h"

extern int id;

Entity *createEntity(char *name, int type, char symbol, Position mapPos, Position gridPos,
                       int maxHP, int ac, int speed, int initiativeMod, int attack, int damage, int gold,
                       int viewRadius, int blind);
Entity *createEntityFromFile(char* entityName, Position mapPos, Position gridPos, int currentHealth);
void setEntityName(Entity *entity, const char* name);


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

// Running entities
int runEntity(RoomGrid *room, Entity *entity, Entity *player);
void runMonster(RoomGrid *room, Entity *entity, Entity *player);
void monsterRoam(RoomGrid *room, Entity *entity, int maxDistance);

// Updates the entity's stats based on the equipment that is equipped
void updateEntityStats(Entity *player);

void updateEntityArmour(Entity *player);
void updateEntityWeapon(Entity *player);

void unequipItem(Entity *player, Item *item);
void unequipWeapon(Entity *player);
void unequipArmour(Entity *player);

#endif
