#ifndef ENTITY_RUNNER_H
#define ENTITY_RUNNER_H

// HEADER FILE WITH ALL RUNNING ENTITY FUNCTIONS
// this was previously in the entity.c file but then it meant that i had to include player.h and monster.h which
// increased the dependencies and made it very unmodular

#include "game.h"

// Runs the entity and returns the status 1 - success 0 - failure
int runEntity(RoomGrid *room, Entity *entity, Entity *player);

#endif