#include <math.h>
#include <stdbool.h>
#include <ncurses/ncurses.h>
#include "game.h"
#include "shadowcasting.h"

#define EPSILON 0.0001f

// Check if a position is within the map boundaries
int is_in_map(RoomGrid *room, int x, int y) {
    return x >= 0 && x < room->width && y >= 0 && y < room->height;
}

// Check if a tile blocks light
int is_blocking(RoomGrid *room, int x, int y) {
    return room->tiles[y][x]->type == WALL;
}

// Compute field of view using recursive shadowcasting
void compute_fov(RoomGrid *room, Entity *player) {
    int player_x = player->gridPos.x;
    int player_y = player->gridPos.y;
    int radius = player->detectionRadius;

    RoomTile ***map = room->tiles;

    // Reset visibility
    for (int y = 0; y < room->height; y++) {
        for (int x = 0; x < room->width; x++) {
            map[y][x]->visible = 0;
        }
    }

    // The player's own tile is always visible
    map[player_y][player_x]->visible = 1;
    map[player_y][player_x]->discovered = 1;

    // Octant transform table
    int mult[8][4] = {
        { 1, 0, 0, 1 },   // N-NE
        { 0, 1, 1, 0 },   // E-NE
        { 0, 1, -1, 0 },  // E-SE
        { -1, 0, 0, 1 },  // S-SE
        { -1, 0, 0, -1 }, // S-SW
        { 0, -1, -1, 0 }, // W-SW
        { 0, -1, 1, 0 },  // W-NW
        { 1, 0, 0, -1 }   // N-NW
    };

    for (int oct = 0; oct < 8; oct++) {
        cast_light(room, player_x, player_y, 1, 1.0, 0.0, radius,
                   mult[oct][0], mult[oct][1], mult[oct][2], mult[oct][3]);
    }
}

// Recursive light casting
void cast_light(RoomGrid *room, int cx, int cy, int row,
                float start_slope, float end_slope, int radius,
                int xx, int xy, int yx, int yy) {
    if (start_slope < end_slope - EPSILON) return;

    RoomTile ***map = room->tiles;

    for (int i = row; i <= radius; i++) {
        int blocked = 0;
        float new_start_slope = start_slope;

        for (int dx = -i, dy = -i; dx <= 0; dx++) {
            dy = -i;

            int ax = cx + dx * xx + dy * xy;
            int ay = cy + dx * yx + dy * yy;

            float l_slope = (dx - 0.5f) / (dy + 0.5f);
            float r_slope = (dx + 0.5f) / (dy - 0.5f);

            if (r_slope > start_slope) continue;
            if (l_slope < end_slope) break;

            if (!is_in_map(room, ax, ay)) continue;

            float distance = sqrtf((float)(dx * dx + dy * dy));
            if (distance <= radius) {
                map[ay][ax]->visible = 1;
                map[ay][ax]->discovered = 1;
            }

            if (blocked) {
                if (is_blocking(room, ax, ay)) {
                    new_start_slope = r_slope;
                    continue;
                } else {
                    blocked = 0;
                    start_slope = new_start_slope;
                }
            }

            if (is_blocking(room, ax, ay) && i < radius) {
                blocked = 1;
                cast_light(room, cx, cy, i + 1, start_slope, l_slope,
                           radius, xx, xy, yx, yy);
                new_start_slope = r_slope;
            }
        }

        if (blocked) break;
    }
}
