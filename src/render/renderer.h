#ifndef RENDERER_H
#define RENDERER_H

// RENDERING!!
#include "../core/game.h"

// Initialises the curses window
void initRenderer();

// Called every frame or frame update to update the window's frame
void draw();

// Closes the renderer
void closeRenderer();

// Draws the room
void drawRoom(RoomGrid *room, int xMin, int xMax, int yMin, int yMax);

// Draws the room with the Entity target centered, with the tiles in radius
void drawRoomWithTarget(RoomGrid *room, Entity *target, int radius);

#endif