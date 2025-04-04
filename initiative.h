#ifndef INITIATIVE_H
#define INIITATIVE_H

#include "game.h"

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

void printCurrentEntityTurn(Entity *entity);

#endif