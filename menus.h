#ifndef MENUS_H
#define MENUS_H

#include "game.h"

// ALL THE MENUS IN THE GAME WILL BE HERE!!!

// STARTING THE GAME MENU
int startGameMenu();
void printGameTitle();

Entity *newGameMenu();
void createPlayerMenu(Entity **player);

int loadGameMenu();

void howToPlayMenu();

void exitGame();

// Room actions menu
int roomActionMenu(RoomGrid *room, Entity *player);

// Combat menu
int attackMenu(RoomGrid *room, Entity *p, Entity **entities, int entityCount);

#endif