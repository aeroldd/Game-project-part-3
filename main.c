#include "game.h"
#include "entity.h"
#include "room.h"
#include "combat.h"
#include "util.h"
#include "dialogue.h"
#include "pathfinding.h"
#include "item.h"
#include "menus.h"
#include <stdlib.h>

void printGameOver(Entity *player);

int runRoom(char* roomName, Entity *player) {
    // Create the room
    RoomGrid *room = createRoomGrid(roomName);

    //printf("room successfully made!");

    //printEntities(room->entities, room->entityCount);

    // Add the player to the entity list
    addEntityToArray(&(room->entities), &(room->entityCount), player);

    // Initialises the initiative list
    InitiativeList list;
    initList(room, &list); // initialises the list
    rollAllInitiatives(&list, room->entities, room->entityCount); // rolls all the entities' initiatives

    // Set the player's position
    setEntityPosition(room, player, (Position){1,1});

    int running = 1;
    int result = 0;

    // DIALOGUE THINGS

    char *keywords[] = {"{username}"};
    char *replacements[] = {player->name};

    playDialogue(room->roomEnterDialogue, keywords, replacements, 1);

    while(running) {
        system("cls");
        // determine initiative order
        Entity *currentEntity = getEntityByCurrentInitiative(&list, room->entities, room->entityCount);
        currentEntity->isCurrentTurn=1;

        //printEntityStats(currentEntity);

        // run the current entity
        if(runEntity(room, currentEntity, player) == 2) {
            // run entity returns 2 when the user choses to finish the room and leave the room
            // leaving room code
            running = 0;
            result = PLAYER_WIN;
        }

        currentEntity->isCurrentTurn=0;
        cycleInitiative(&list);

        // Check if the player died
        if(player->currentHP<=0) {
            running = 0;
            result = PLAYER_LOSE;
        }
    }

    if(result == PLAYER_LOSE) {
        printGameOver(player);
    }
    if(result == PLAYER_WIN) {
        playDialogue(room->roomClearDialogue, keywords, replacements, 1);
    }

    return 0;
}

void printGameOver(Entity *player) {
    system("cls");
    printf("==========\n");
    fancyPrint("GAME OVER!\n");
    printf("==========\n");
    fancyPrint("%s final stats:\n", player->name);
    fancyPrint("MAX HP: %d\n", player->name);
    fancyPrint("ARMOUR: %d\n", player->ac);
    fancyPrint("GOLD: %d\n", player->gold);
    printf("==========\n");
}


int main() {
    // Initialise and create the player
    Entity *player = NULL;

    // Menu things
    int menuChoice = startGameMenu();

    switch(menuChoice) {
        case 0: {
            exitGame();
            break;
        }
        // NEW GAME MENU
        case 1: {
            player = newGameMenu();
            runRoom("maze_room.txt", player);
        }
    }

    pressAnyKey();

    //runRoom("maze_test.txt", player);
    return 0;
}
