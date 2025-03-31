#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "game.h"

// Returns the next position for the monster to move towards the target (player).
// If no valid path is found, returns the monster's current position.
Position* findShortestPath(RoomGrid *room, Position start, Position target, int *pathLength);

#endif