#ifndef COMBAT_H
#define COMBAT_H

#include "game.h"
#include "room.h"

// Damages entity for the amount of damage indicated
int damageEntity(Entity *e, int damage);

// Attack
int attack(RoomGrid *room, Entity *attacker, Entity *reciever);

// Rolls a die of size max
int roll(int max);

// Rolls a 20 sided die with checking for critical success or failure 
int rolld20();

// Calculates the attack roll of an entity
int attackRoll(Entity *e);

void printAttackMessage(Entity *attacker, Entity *reciever);
void printAttackSuccess(Entity *attacker, Entity *reciever, int damage);
void printAttackFailure(Entity *attacker, Entity *reciever);
void printKillMessage(Entity *attacker, Entity *reciever);
void printHealthLeft(Entity *entity);

// Initialises the initiative list - sets the InitiativeList to the room's initiativeList attribute
void initList(RoomGrid *room, InitiativeList *list);

// Rolls initiative for all entities in the entity list
void rollAllInitiatives(InitiativeList *initiativeList, Entity **entities, int entityCount);

// Rolls initiative for an entity, updates the initiative stat
void rollInitiative(Entity *entity);

// Insert index into linked list in initiative order
void insertInOrder(InitiativeList *list, Entity **entities, int entityCount, int entityId);

// Print initiative order using entity array
void printInitiativeOrder(InitiativeList *list, Entity **entities, int entityCount);

// Removes entity from the initiative list
void removeEntityInitiative(InitiativeList *list, int entityIndex);

// Returns the current entity depending on the current initiative number
Entity *getEntityByCurrentInitiative(InitiativeList *list, Entity **entities, int entityCount);

void cycleInitiative(InitiativeList *list);

// Free the list
void freeList(InitiativeList *list);

// Calculates if an attack lands by comparing the attack roll and the reciever's ac
int calculateHit(int attack, int ac);

// Kills an entity -> removes it from the map, the list, and frees the memory
void kill(RoomGrid *room, Entity *attacker, Entity *reciever);
void removeEntityFromMap(RoomGrid *room, Entity *e);

// Rewards an entity with gold
void reward(Entity *e, int gold);

void printCurrentEntityTurn(Entity *entity);



#endif