#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "core/game.h"
#include "world/room.h"
#include "gameplay/combat.h"
#include "gameplay/initiative.h"
#include "entity/entity.h"
#include "dialogue/dialogue.h"
#include "gameplay/menus.h"
#include "util/util.h"
#include "entity/entity_runner.h"
#include "world/map.h"


int runRoom(RoomGrid *room, Entity *player) {
    // Create the room
    printf("room successfully made!");

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

    // the value x means theres no dialogue 
    if(!strcmp(room->roomEnterDialogue, "x")) {
        char *keywords[] = {"{username}"};
        char *replacements[] = {player->name};
    
        playDialogue(room->roomEnterDialogue, keywords, replacements, 1);
    }
    else {
        fancyPrint("%s enters %s\n", player->name, room->name);
    }


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
        if(!strcmp(room->roomEnterDialogue, "x")) {
            char *keywords[] = {"{username}"};
            char *replacements[] = {player->name};
            playDialogue(room->roomClearDialogue, keywords, replacements, 1);
        }
        fancyPrint("YOU WIN!\n");
    }

    return 0;
}

int main() {
    // Set the random seed!
    srand(time(NULL));

    // Initialise and create the player
    Entity *player = NULL;

    // Initialise a map
    Map *map = createMap();

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
            RoomGrid *room = createRoomGrid("starter_room.txt");
            //printRoomDetails(room);
            addRoomToMap(map, room);
            printRoomsinMap(map);

            pressAnyKey();
            runRoom(room, player);
        }
    }

    pressAnyKey();

    //runRoom("maze_test.txt", player);
    return 0;
}
