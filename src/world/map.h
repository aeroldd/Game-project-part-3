#ifndef MAP_H
#define MAP_H

#include <stdlib.h>

#include "../core/game.h"

extern int id;

Map *createMap();
void addRoomToMap(Map *map, RoomGrid *room);

void addRoomIdToMap(Map *map, int id);

void setCurrentRoomId(Map *map, int id);
void setCurrentRoom(Map *map, RoomGrid *room);

RoomGrid *getRoomById(Map *map, int id);

void printRoomsinMap(Map *map);

void loadRoomFolder(Map *map);

void changeRoom(Map *map, int roomId, Entity *entity, Position newPos);

#endif