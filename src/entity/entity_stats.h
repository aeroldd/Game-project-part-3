#ifndef ENTITY_STATS_H
#define ENTITY_STATS_H

#include "game.h"

// Updates the entity's stats based on the equipment that is equipped
void updateEntityStats(Entity *player);

void updateEntityArmour(Entity *player);
void updateEntityWeapon(Entity *player);

void unequipItem(Entity *player, Item *item);
void unequipWeapon(Entity *player);
void unequipArmour(Entity *player);

#endif