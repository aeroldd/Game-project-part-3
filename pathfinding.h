#ifndef PATHFINDING_H
#define PATHFINDING_H

#include "game.h"

// Returns the next position for the monster to move towards the target (player).
// If no valid path is found, returns the monster's current position.
Position* findShortestPath(RoomGrid *room, Position start, Position target, int *pathLength);

// Returns 1 if the positions are in line of sight of eachother
int lineOfSight(Position pos1, Position pos2, int radius, RoomGrid *room);

#endif