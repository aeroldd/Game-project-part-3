/* ASCII ART AND TILE RENDERER FOR MY GAME USING NCURSES*/

#include "renderer.h"
#include <ncurses/ncurses.h>

#include "game.h"

// Forward declare the private (??? the ones that arent in the header file)
// functions

void initColors();
void placeTileSymbol(RoomTile *tile);

// Initialises the curses window
void initRenderer() {
    initscr(); // initialises the ncurses window
    raw();
    noecho();

    // Initialise colour if the terminal supports color
    if(has_colors()) {
        initColors();
    }
}

// Initialises different color pairs
void initColors() {
    start_color();
    // Initialising colour pairs
    init_pair(1, COLOR_WHITE,  COLOR_BLACK);   // visible floor
    init_pair(2, COLOR_BLACK,  COLOR_WHITE);   // highlighted
    init_pair(3, COLOR_BLUE,   COLOR_BLACK);   // player
    init_pair(4, COLOR_CYAN,   COLOR_BLACK);   // discovered (grey)
    init_pair(5, COLOR_RED,    COLOR_BLACK);   // enemies
    init_pair(6, COLOR_YELLOW, COLOR_BLACK);   // current turn
}

// draw() - called in the main loop, or when the screen needs to be updated
void draw() {
    refresh();
}

void closeRenderer() {
    endwin();
}

void placeTileSymbol(RoomTile *tile) {
    mvaddch(tile->pos.x, tile->pos.y, tile->symbol);
}
// places a tile at position pos using ncurses mvprint if the tile is visible / discovered
void placeTile(RoomTile *tile) {
    int x = tile->pos.x;
    int y = tile->pos.y;

    // Check if the tile is visible or discovered
    if(tile->visible) {
        attron(COLOR_PAIR(1));
        placeTileSymbol(tile);
        attroff(COLOR_PAIR(1));

        // update the tile to discovered if it is visible
        tile->discovered = 1;
        return;
    }
    if(tile->discovered) {
        attron(COLOR_PAIR(4));
        placeTileSymbol(tile);
        attroff(COLOR_PAIR(4));
        return;
    }

    // if the tile isnt visible or discovered it will just print a blank character
    mvaddch(tile->pos.x, tile->pos.y, ' ');
}

// draws the map array of a room
void placeTiles(RoomTile ***tiles, int xMin, int xMax, int yMin, int yMax) {
    for(int y = yMin; y < yMax; y++) {
        for(int x = xMin; x < xMax; x++) {
            placeTile(tiles[y][x]);
        }
    }
}

// draws an entity at their grid positions
void drawEntity(Entity *entity) {
    // Determine what the entity is and have a different color for each enemy
    if(entity->isCurrentTurn) {
        attron(COLOR_PAIR(6));
    }
    else if(entity->selected) {
        attron(COLOR_PAIR(2));
    }
    else if(entity->type == PLAYER){ 
        attron(COLOR_PAIR(3));
    }
    else if (entity->type == MONSTER) {
        attron(COLOR_PAIR(5));
    }
    // moves the cursor at the entity's grid position and puts their symbol
    mvaddch(entity->gridPos.x, entity->gridPos.y, entity->symbol);
    // reset the attributes
    attrset(A_NORMAL);
}

// draws the entity array in certain bounds
void drawEntities(Entity **entities, int entityCount, int xMin, int xMax, int yMin, int yMax) {
    for(int i = 0; i < entityCount; i++) {
        // If the entity is in bounds
        if(entities[i]->gridPos.x > xMin && entities[i]->gridPos.x < xMax
        && entities[i]->gridPos.y > yMin && entities[i]->gridPos.y < yMax) {
            drawEntity(entities[i]);
        }
    }
}

// draws the room
void drawRoom(RoomGrid *room, int xMin, int xMax, int yMin, int yMax) {
    RoomTile ***tiles = room->tiles;
    placeTiles(tiles, xMin, xMax, yMin, yMax);
    drawEntities(room->entities, room->entityCount, xMin, xMax, yMin, yMax);
}

// draws the room around a targeted entity with a radius
void drawRoomWithTarget(RoomGrid *room, Entity *target, int radius) {
    int xMin = MAX(target->gridPos.x - radius, 0);
    int xMax = MIN(target->gridPos.x + 1 + radius, room->width);
    int yMin = MAX(target->gridPos.y - radius, 0);
    int yMax = MIN(target->gridPos.y + 1 + radius, room->height);

    drawRoom(room, xMin, xMax, yMin, yMax);
    drawEntities(room->entities, room->entityCount, xMin, xMax, yMin, yMax);
}
