#ifndef MAP_H
#define MAP_H

#include <stdlib.h>

#include "../core/game.h"

extern int id;

Map *createMap();
void addRoomToMap(Map *map, RoomGrid *room);

void printRoomsinMap(Map *map);

#endif