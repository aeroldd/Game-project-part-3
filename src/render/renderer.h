#ifndef RENDERER_H
#define RENDERER_H

// RENDERING!!
#include <ncurses/curses.h>
#include "../core/game.h"

// Initialises the curses window
void initRenderer();

// Called every frame or frame update to update the window's frame
void draw();

// Closes the renderer
void closeRenderer();

// Draws the room
void drawRoom(WINDOW *mapwin, RoomGrid *room, int xMin, int xMax, int yMin, int yMax, Position offset);

// Draws the room with the Entity target centered, with the tiles in radius
void drawRoomWithTarget(WINDOW *mapwin, RoomGrid *room, Entity *target, int radius);

WINDOW *createRoomGridWindow(int y, int x);

void displayGameTitle(int y, int x);

void renderStats(int y, int x, Entity *entity);

void renderInventory(int y, int x, Inventory *inventory);

int renderActionMenu(int y, int x, RoomGrid *room, Entity *entity, char key, int canAttack);
void handleActionMenu(int action, int *inCombatMenu);
int renderCombatMenu(int y, int x, RoomGrid *room, Entity *entity, Entity **targets, int targetCount, int currentTarget, char key);
void handleCombatMenu(int option, int *inCombatMenu);

void highlightMenuOption(WINDOW *win, char *menuOptions[], int length, int menuIdex);

#endif