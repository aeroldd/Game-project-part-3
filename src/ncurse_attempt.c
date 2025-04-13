#include <ncurses/ncurses.h>
#include <stdio.h>

#include "core/game.h"
#include "render/renderer.h"
#include "render/shadowcasting.h"
#include "world/room.h"
#include "entity/entity.h"
#include "entity/player.h"
#include "util/util.h"
#include "gameplay/combat.h"

// THIS FILE IS TO TEST THE FOLLOWING
// room generation
// the renderer features

int loop(WINDOW *win, RoomGrid *room, Entity *player);

int main() {
    // Room creation and player creation testing
    RoomGrid *room = createRoomGrid("maze_room.txt");
    Entity *player = createPlayerEntity("Test", '@', (Position) {2,2}, (Position) {5,5});
    addEntityToArray(&room->entities, &room->entityCount, player);

    // Create a window for the grid

    initRenderer();

    WINDOW *gridWindow = createRoomGridWindow(4,1);

    box(gridWindow, 0, 0);

    refresh();

    // START DRAWING HERE

    printw("%s", player->name);

    //displayGameTitle(0,0);
    renderStats(5,22,player);    

    compute_fov(room, player);
    drawRoomWithTarget(gridWindow,room, player, 8);
    //renderInventory(17,22, player->inventory);

    renderActionMenu(17, 22, room, player, 0, 0);
    wrefresh(gridWindow);

    // MAIN GAME LOOP GOES HERE
    while(loop(gridWindow, room, player));

    char key = getch();

    delwin(gridWindow);

    closeRenderer();

    return 0;
}

int loop(WINDOW *win, RoomGrid *room, Entity *player) {
    char key = getch();

    werase(win);

    // If in combat, no player inputs

    
    // get the number of entities around the player to determine if the player can attack
    Entity **targets = getTargets(room, player);
    int targetCount = getEntityCount(targets);
    int currentTargetIndex = 0;

    int inCombatMenu = 0;
    if(!inCombatMenu) {
        char key = getch();
        player->distanceLeft = player->speed;
        if(movePlayer(room, player, key)) compute_fov(room, player); 

        // Determining actions
        int action;

        action = renderActionMenu(17,22, room, player, key, targetCount);
        handleActionMenu(action, &inCombatMenu);
    }
    else {
        char key = getch();
        // Inside the combat menu

        // Determine the combat option
        int option;

        option = renderCombatMenu(23, 22, room, player, targets, targetCount, currentTargetIndex, key);
        handleCombatMenu(option, &inCombatMenu);
    }


    // if(inCombatMenu) {
    //     key = getch();
    //     int combatOption = renderCombatMenu(21, 22, room, player, targets, targetCount, targetIndex, key);

    //     switch (combatOption) {
    //         case 0: {
    //             inCombatMenu = 0;
    //             break;
    //         }
    //         //Case 1: The player attacks the targetted entity1
    //         case 1: {
    //             int damage = attack(room, player, targets[targetIndex]);
    //             if (damage) {
    //                 if (damage == -100) {
    //                     printw("KILLED THE ENEMY!");
    //                     break;
    //                 }
    //                 printw("HIT THE ENEMY! BOOYAH! dealt %d damage.", damage);               
    //                 break;         
    //             }
    //             printw("MISSED THE ENEMY!");
    //             break;
    //         }
    //     }
    // }

    drawRoomWithTarget(win, room, player, 10);          
    renderStats(5,22,player);    
    renderInventory(13, 22, player->inventory);

    if(key == KEY_EXIT) return 0;

    wrefresh(win);
    refresh();
    return 1;
}