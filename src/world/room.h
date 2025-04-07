#ifndef ROOM_H
#define ROOM_H

#include "../core/game.h"

RoomGrid *createRoomGrid(char* roomName);

RoomTile ***createRoomTilesArray(RoomGrid *room);
RoomTile *createRoomTile(Position pos, char symbol);

int getDistancePos(Position pos1, Position pos2);
int checkValidPosition(RoomGrid room, Position pos);

Entity **getEntitiesAroundPoint(RoomGrid *room, Position targetPos);

int isRoomCleared(RoomGrid *room);

void printRoomCleared(RoomGrid *room, Entity *player);

#endif
