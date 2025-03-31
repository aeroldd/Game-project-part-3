#ifndef ROOM_H
#define ROOM_H

#include "game.h"

RoomTile ***createRoomTilesArray(RoomGrid *room);
RoomTile *createRoomTile(Position pos, char symbol);
void displayRoom(RoomGrid *room);
void displayRoomWithPlayerCamera(RoomGrid *room, Entity *p, int camSize);
void displayMoveMenu();
int getDistancePos(Position pos1, Position pos2);
int checkValidPosition(RoomGrid room, Position pos);
RoomGrid *createRoomGrid(char* roomName);
int moveMenu(RoomGrid *room, Entity *p);
int roomActionMenu(RoomGrid *room, Entity *player);
Entity **getEntitiesAroundPoint(RoomGrid *room, Position targetPos);

int isRoomCleared(RoomGrid *room);
void printRoomCleared(RoomGrid *room, Entity *player);

#endif
