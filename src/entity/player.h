#ifndef PLAYER_H
#define PLAYER_H

#include "../core/game.h"

// Creates a player entity
Entity *createPlayerEntity(char *name, char symbol, Position mapPos, Position gridPos);

// Moves the player depending on the keyboard input it recieves
int movePlayer(RoomGrid *room, Entity *player, char key);



#endif