#ifndef PLAYER_H
#define PLAYER_H

#include "game.h"
#include "entity.h"
#include "util.h"
#include "room.h"

// Moves the player depending on the keyboard input it recieves
int movePlayer(RoomGrid *room, Entity *player, char key);



#endif