#ifndef COMBAT_H
#define COMBAT_H

#include "../core/game.h"

// Damages entity for the amount of damage indicated
int damageEntity(Entity *e, int damage);

// Attack
int attack(RoomGrid *room, Entity *attacker, Entity *reciever);

// Calculates the attack roll of an entity
int attackRoll(Entity *e);

void printAttackMessage(Entity *attacker, Entity *reciever);
void printAttackSuccess(Entity *attacker, Entity *reciever, int damage);
void printAttackFailure(Entity *attacker, Entity *reciever);
void printKillMessage(Entity *attacker, Entity *reciever);
void printHealthLeft(Entity *entity);

// Calculates if an attack lands by comparing the attack roll and the reciever's ac
int calculateHit(int attack, int ac);

// Kills an entity -> removes it from the map, the list, and frees the memory
void kill(RoomGrid *room, Entity *attacker, Entity *reciever);
void removeEntityFromMap(RoomGrid *room, Entity *e);

// Rewards an entity with gold
void reward(Entity *e, int gold);

// Check if the entity has valid targets around it which it can attack - returns the number of targets
Entity **getTargets(RoomGrid *room, Entity *entity);


#endif