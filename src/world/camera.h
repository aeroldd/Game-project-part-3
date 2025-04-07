#ifndef CAMERA_H
#define CAMERA_H

// Camera and displaying functions!

#include "game.h"

void displayRoom(RoomGrid *room);
void displayRoomWithPlayerCamera(RoomGrid *room, Entity *p, int camSize);

#endif