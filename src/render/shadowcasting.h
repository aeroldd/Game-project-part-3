#ifndef SHADOWCASTING_H
#define SHADOWCASTING_H

#include "game.h"

// Function prototypes
void compute_fov(RoomGrid *room, Entity *player);
void cast_light(RoomGrid *room, int cx, int cy, int row, float start_slope, float end_slope, int radius, int xx, int xy, int yx, int yy);
int is_in_map(RoomGrid *room, int x, int y);
int is_blocking(RoomGrid *room, int x, int y);

#endif