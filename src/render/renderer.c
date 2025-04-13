/* ASCII ART AND TILE RENDERER FOR MY GAME USING NCURSES*/

#include "renderer.h"
#include <ncurses/ncurses.h>
#include <stdio.h>
#include <string.h>

#include "game.h"
#include "../util/util.h"

// Forward declare the private (??? the ones that arent in the header file)
// functions

void initColors();
void placeTileSymbol(WINDOW *mapwin, RoomTile *tile, Position offset);

// Initialises the curses window
void initRenderer() {
    initscr(); // initialises the ncurses window
    raw();
    noecho();
    //cbreak();
    nodelay(stdscr, 1);

    //test

    // Initialise colour if the terminal supports color
    if(has_colors()) {
        initColors();
        attron(COLOR_PAIR(2));
        printw("COLORS ON");
        attroff(COLOR_PAIR(2));
    }
    else {
        printw("COLOR NOT AVAILABLE");
    }

    refresh(); // refresh here BECAUSE THE CHILDREN WONT WORK WITHOUT THE PARENT REFRESHING (took me 1 hour to find out :(()))
    getch();
}

// Initialises different color pairs
void initColors() {
    start_color();
    // Initialising colour pairs
    init_pair(1, COLOR_WHITE,  COLOR_BLACK);   // visible floor
    init_pair(2, COLOR_BLACK,  COLOR_WHITE);   // highlighted
    init_pair(3, COLOR_BLUE,   COLOR_BLACK);   // player
    init_pair(4, COLOR_CYAN, COLOR_BLACK);   // discovered (grey)
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

void placeTileSymbol(WINDOW *mapwin, RoomTile *tile, Position offset) {
    wmove(mapwin, tile->pos.y + offset.y, tile->pos.x + offset.x);
    waddch(mapwin, tile->symbol);
}
// places a tile at position pos using ncurses mvprint if the tile is visible / discovered
void placeTile(WINDOW *mapwin, RoomTile *tile, Position offset) {
    int x = tile->pos.x;
    int y = tile->pos.y;

    // Check if the tile is visible or discovered
    if(tile->visible) {
        wattron(mapwin, COLOR_PAIR(1));
        placeTileSymbol(mapwin, tile, offset);
        wattroff(mapwin, COLOR_PAIR(1));

        // update the tile to discovered if it is visible
        tile->discovered = 1;
        return;
    }
    if(tile->discovered) {
        wattron(mapwin, COLOR_PAIR(4));
        placeTileSymbol(mapwin, tile, offset);
        wattroff(mapwin, COLOR_PAIR(4));
        return;
    }

    // if the tile isnt visible or discovered it will just print a blank character
    mvwaddch(mapwin, tile->pos.y + offset.y, tile->pos.x + offset.x, ' ');
}

// draws the map array of a room
void placeTiles(WINDOW *mapwin, RoomTile ***tiles, int xMin, int xMax, int yMin, int yMax, Position offset) {
    for(int y = yMin; y < yMax; y++) {
        for(int x = xMin; x < xMax; x++) {
            placeTile(mapwin, tiles[y][x], offset);
        }
    }
}

// draws an entity at their grid positions
void drawEntity(WINDOW *mapwin, Entity *entity, Position offset) {
    // Check if the entity is visible

    // Determine what the entity is and have a different color for each enemy
    if(entity->isCurrentTurn) {
        wattron(mapwin, COLOR_PAIR(6));
    }
    else if(entity->selected) {
        wattron(mapwin, COLOR_PAIR(2));
    }
    else if(entity->type == PLAYER){ 
        wattron(mapwin, COLOR_PAIR(3));
    }
    else if (entity->type == MONSTER) {
        wattron(mapwin, COLOR_PAIR(5));
    }
    // moves the cursor at the entity's grid position and puts their symbol

    // Move the cursor at the map grid to the entity's grid position (+ the offset)
    wmove(mapwin, entity->gridPos.y + offset.y, entity->gridPos.x + offset.x);
    waddch(mapwin, entity->symbol);

    // reset the attributes
    wattrset(mapwin, A_NORMAL);
}

// draws the entity array in certain bounds
void drawEntities(WINDOW *mapwin, RoomGrid *room, Entity **entities, int entityCount, int xMin, int xMax, int yMin, int yMax, Position offset) {
    RoomTile ***tiles = room->tiles;
    for (int i = 0; i < entityCount; i++) {
        int ex = entities[i]->gridPos.x;
        int ey = entities[i]->gridPos.y;
        if (ex > xMin && ex < xMax && ey > yMin && ey < yMax) {
            // Debug output to check visibility
            //mvprintw(15 + ey, 60, "Entity at (%d,%d) Visible: %d", ex, ey, tiles[ey][ex]->visible);
            if (tiles[ey][ex]->visible) {
                drawEntity(mapwin, entities[i], offset);
            }
        }
    }
}

// draws the room
void drawRoom(WINDOW *mapwin, RoomGrid *room, int xMin, int xMax, int yMin, int yMax, Position offset) {
    RoomTile ***tiles = room->tiles;

    placeTiles(mapwin, tiles, xMin, xMax, yMin, yMax, offset);
    drawEntities(mapwin, room, room->entities, room->entityCount, xMin, xMax, yMin, yMax, offset);
}

// draws the room around a targeted entity with a radius
void drawRoomWithTarget(WINDOW *mapwin, RoomGrid *room, Entity *target, int radius) {
    int xMin = MAX(target->gridPos.x - radius, 0);
    int xMax = MIN(target->gridPos.x + 1 + radius, room->width);
    int yMin = MAX(target->gridPos.y - radius, 0);
    int yMax = MIN(target->gridPos.y + 1 + radius, room->height);

    // find the mid point of the window as a position
    Position midPoint = (Position) {20/2, 20/2};

    // find the different between the player's position and the midpoint of the screen's position. this is the offset
    Position offset = (Position) {midPoint.x - target->gridPos.x , midPoint.y - target->gridPos.y};

    drawRoom(mapwin, room, xMin, xMax, yMin, yMax, offset);
    drawEntities(mapwin, room, room->entities, room->entityCount, xMin, xMax, yMin, yMax, offset);

    box(mapwin, 0, 0);
}

WINDOW *createRoomGridWindow(int y, int x) {
    WINDOW *mapwin = newwin(20, 20, y, x);
    box(mapwin,0,0);
    wrefresh(mapwin);
    return mapwin;
}

void updateMapWindow(WINDOW *map) {
    wrefresh(map);
}

void displayGameTitle(int y, int x) {
    WINDOW *gameTitleWin = newwin(3, 14, y, x);
    box(gameTitleWin, 0, 0);
    mvwprintw(gameTitleWin, 1, 1, "DUNGEON.txt");
    wrefresh(gameTitleWin);
}

void renderStats(int y, int x, Entity *entity) {
    WINDOW *win = newwin(8, 20, y, x);
    box(win, 0, 0);
    mvwprintw(win, 1, 1, "%s's stats:", entity->name);
    mvwprintw(win, 2, 1, "HP: %d/%d", entity->currentHP, entity->maxHP);
    mvwprintw(win, 3, 1, "SPD: %d (%d left)", entity->speed, entity->distanceLeft);
    mvwprintw(win, 4, 1, "GOLD: %d", entity->gold);

    char *weaponName = entity->weapon==NULL? "Fists" : entity->weapon->base.name;
    mvwprintw(win, 6, 1, "WEAPON: %s", weaponName);

    wrefresh(win);
}

void renderInventory(int y, int x, Inventory *inventory) {
    // Create the window for the inventory
    WINDOW *win = newwin(8, 32, y, x);
    box(win, 0, 0);

    wmove(win, 1, 1);

    char inventoryName[32];

    if(!inventory) {
        sprintf(inventoryName, "INVENTORY");
    }
    else sprintf(inventoryName, "%s", inventory->owner->name);

    mvwprintw(win,1,1, "%s\n", inventoryName);

    if(inventory->itemCount == 0) {
        mvwprintw(win, 2,1, "<%s's inventory is empty>", inventory->owner->name);
    }

    for(int i = 0; i < inventory->itemCount; i++) {
        // if there isnt an item at this slot print empty
        if(!inventory->items[i]) wprintw(win, " %d - <empty slot>", i + 1);
        wprintw(win, "%d %s\n", i + 1, inventory->items[i]->name);
    }

    wrefresh(win);
}

// A menu option struct for keybindings and their menu option

typedef struct{
    char key;
    char label[32];
    int returnValue;
} MenuOption;

void printOptionsDebugger(char **actionList, int length, int menuIndex) {
    //printw("action list is:\n");
    // for(int i = 0; i < length; i++) {
    //     printw("%d - %s\n", i, actionList[i]);
    // }

    printw("At menuIndex %d ", menuIndex);
    printw("Current action: %s", actionList[menuIndex]);
}

int renderActionMenu(int y, int x, RoomGrid *room, Entity *player, char key, int targets) {
    // Create the action menu window
    WINDOW *win = newwin(10, 24, y, x);
    box(win, 0, 0);

    // Title
    mvwprintw(win, 1,1, "ACTIONS:");

    MenuOption actions[] = {
        {'0', "SKIP [0]", 0},
        {'1', "ATTACK [1]", 1},
        {'2', "INVENTORY [2]", 2},
        {'3', "INTERACT [3]", 3}
    };

    MenuOption actionList[4];
    int actionCount = 0;

    // Always add the skip action
    actionList[actionCount++] = actions[0];

    // Add the attack action only if there are adjecent targets
    if(targets) {
        actionList[actionCount++] = actions[1];
    }

    // Always add the inventory action
    actionList[actionCount++] = actions[2];

    // Interact if next to interactable item (work on this later)
    actionList[actionCount++] = actions[3];

    int menuOption = -1;

    // Print the actions in the action list
    for (int i = 0; i < actionCount; i++) {
        if(key == actionList[i].key) {
            // If the key is equal to the key of the action from the action list:
            // Highlight the label

            wattron(win, COLOR_PAIR(2));
            mvwprintw(win, i+2, 1, "%s", actionList[i].label);
            wattroff(win, COLOR_PAIR(2));

            // set the menu option to the return value
            menuOption = actionList[i].returnValue;
        }
        else 
        mvwprintw(win, i+2, 1, "%s", actionList[i].label);
    }

    wrefresh(win);
    return menuOption;
}

void handleActionMenu(int action, int *inCombatMenu){
    switch (action) {
        // COMBAT MENU
        case 1: {
            *inCombatMenu = 1;
            break;
        }
        // Inventory
        case 2: {
            break;
        }
        // Interaction menu
        case 3: {
            break;
        }
        // SKIPPING
        case 0: {
            break;
        }
    }
}

int renderCombatMenu(int y, int x, RoomGrid *room, Entity *entity, Entity **targets, int targetCount, int currentTarget, char key) {
    // Create the window
    WINDOW *win = newwin(6, 20, y, x);
    box(win, 0, 0);

    // determine the target
    Entity *target = targets[currentTarget];

    // Combat menu action list

    char attackOption[32];
    sprintf(attackOption, "[1] ATTACK %s", target->name);

    MenuOption actionList[] = {
        {'1', "", 1},
        {'2', "[2] CHANGE TARGET", 2},
        {'0', "[0] EXIT MENU", 0}
    };

    // Need to use string copying because you cant just assign attackOption like that 
    strncpy(actionList[0].label, attackOption, sizeof(actionList[0].label)-1);

    mvwprintw(win, 1,1,"COMBAT MENU");

    int menuOption = -1;
    // Print the actions in the action list
    for (int i = 0; i < 3; i++) {
        if(key == actionList[i].key) {
            // If the key is equal to the key of the action from the action list:
            // Highlight the label

            wattron(win, COLOR_PAIR(2));
            mvwprintw(win, i+2, 1, "%s", actionList[i].label);
            wattroff(win, COLOR_PAIR(2));

            // set the menu option to the return value
            menuOption = actionList[i].returnValue;
        }
        else 
        mvwprintw(win, i+2, 1, "%s", actionList[i].label);
    }

    wrefresh(win);
    return menuOption;
}

void handleCombatMenu(int option, int *inCombatMenu) {
    switch(option){
        // case 1: attack the current target
        // attacking
    }
}

void highlightMenuOption(WINDOW *win, char* menuOptions[], int length, int menuIndex) {
    wattron(win, COLOR_PAIR(2));
    mvwprintw(win, menuIndex + 1, 1, "%s", menuOptions[menuIndex-1]);
    wattroff(win, COLOR_PAIR(2));
    wrefresh(win);
}

// WINDOW *createDialogueWindow(int y, int x) {
//     WINDOW *win = newwin(64, 7)
// }

// void renderActionMenu(int y, int x, RoomGrid *room, Entity *player) {
//     // Reset the player's distance left
//     player->distanceLeft = player->speed;

//     // The room action menu will keep running until the distance left is 0 and one attack has been done
//     int runningRoomActionMenu = 1;
//     int attacked = 0;

//     int inventorySelected = 0;
//     int selectedItemIndex = 0;

//     while(runningRoomActionMenu) {
//         system("cls");
//         printEntityStats(player);
//         displayRoomWithPlayerCamera(room, player, 5);
//         printf("====================\n");
//         printInventoryBar(player, inventorySelected, selectedItemIndex);
//         printf("====================\n");
//         printf("ACTIONS\n");
//         printf("====================\n");

//         if(player->distanceLeft != 0) {
//             printf("MOVE: WASD (distance left: %d)\n", player->distanceLeft);
//         }
        
//         if(!isRoomCleared(room)) {
//             if(player->distanceLeft == 0 && attacked) {
//                 printf("SKIP TURN (all actions have been used) [0]");
//             }
//             else printf("SKIP TURN [0]\n");
//         }
//         else printf("FINISH ROOM: [0]\n");

//         // Checking if the player can attack (There are monsters in range)
//         Entity **entities = getEntitiesAroundPoint(room, player->gridPos);
//         int entityCount = 0, inAttackRange = 0;
//         if (entities) {
//             for (int i = 0; entities[i] != NULL; i++) {
//                 if (entities[i]->type == MONSTER) {
//                     inAttackRange = 1;
//                     entityCount++;
//                 }
//             }
//         }

//         if(!inventorySelected) {
//             if(inAttackRange && !attacked) {
//                 printf("ATTACK: [1]\n");
//             }
//             else if(attacked && inAttackRange) {
//                 printf("ALREADY USED UP ALL ATTACKS.\n");
//             }
//         }

//         if(!inventorySelected) printf("INVENTORY: [2]\n");

        
//         // Inventory actions
//         // Get the current selected item
//         Item *currentItem = getItemFromInventoryIndex(player->inventory, selectedItemIndex);
//         if(inventorySelected) {
//             if(currentItem->type == WEAPON || currentItem->type == ARMOUR) {
//                 // if the item is equipped, unequip item, else equip item
//                 if(((player->weapon == NULL)  && (currentItem->type == WEAPON)) || ((player->armour == NULL) && (currentItem->type == ARMOUR))) {
//                     printf("EQUIP %s: [5]\n", currentItem->name);
//                 }
//                 else printf("UNEQUIP %s: [5]\n", currentItem->name);
//             }
//             else printf("CONSUME %s: [5]\n", currentItem->name);
//             printf("DROP %s: [6]\n", currentItem->name);
//             printf("EXIT INVENTORY MODE: [x]\n");
//         }

//         char key = getKeyPress();
    
//         switch(key) {
//             case 'w':
//             case 'a':
//             case 's':
//             case 'd':
//             {
//                 movePlayer(room, player, key);
//                 break;
//             }
//             // The user skips the player's turn
//             case '0': {
//                 // if the room isnt cleared and 0 is pressed, the turn is skipped
//                 if(!isRoomCleared(room)) {
//                     fancyPrint("Skipping %s's turn.\n", player->name);
//                     runningRoomActionMenu = 0;
//                     break;
//                 }

//                 // if the room is cleared and 0 is pressed, the room is completed!
//                 return 2;

//                 break;
//             }
//             case '1': {
//                 // If inventory mode isnt selected, number 1 will attack enemies
//                 if(!inventorySelected) {
//                     // Check if there are monsters in range
//                     if(!inAttackRange) {
//                         fancyPrint("There are no monsters in range.\n");
//                         continue;
//                     }
//                     // Check if the player has already used their attack
//                     if(!attacked) {
//                         attacked = attackMenu(room, player, entities, entityCount);
//                         continue;
//                     }
//                     printf("You already used your attack!\n");
//                     break;
//                 }

//                 // Inventory mode selected!
//                 // Set the selected item to 0
//                 selectedItemIndex = 0;

//                 break;
//             }
//             case '2': {
//                 // Inventory!!
//                 if(!inventorySelected) {
//                     inventorySelected = 1;
//                     break;
//                 }

//                 // Inventory mode is selected!
//                 // Set the selected item to 1

//                 selectedItemIndex = 1;

//                 break;
//             }

//             case '3': {
//                 if(inventorySelected) {
//                     selectedItemIndex = 2;
//                 }
//                 break;
//             }

//             case '4': {
//                 if(inventorySelected) {
//                     selectedItemIndex = 3;
//                 }
//                 break;
//             }

//             // use item
//             case '5': {
//                 if(!inventorySelected) break;
//                 // Use the item at the current selected item index
//                 if(!currentItem) {
//                     printf("There is no item at index %d", selectedItemIndex);
//                     break;
//                 }
//                 useItem(player, currentItem);
//                 break;
//             }

//             // drop item
//             case '6': {
//                 if (!inventorySelected) break;
//                 // Drop the item at the current selected item index
//                 deleteItemFromInventory(player, currentItem, 1);
//                 //unequipItem(player, currentItem);
//             }

//             // read item description
//             case '7': {
//                 if (!inventorySelected) break;
//                 // Read the item's description at the selected item index
//             }

//             case 'x': {
//                 inventorySelected=0;
//             }

//         }
//     }
//     system("cls");
//     printEntityStats(player);
//     displayRoomWithPlayerCamera(room, player, 5);
//     printf("====================\n");
//     fancyPrint("%s's turn has ended!\n", player->name);
//     printf("====================\n");
//     delay(500);
//     pressAnyKey();
//     return 1;