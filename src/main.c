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
#include "render/shadowcasting.h"

int runRoom(RoomGrid *room, Entity *player) {
    // Create the room
    printf("room successfully made!");

    //printEntities(room->entities, room->entityCount);

    // Initialises the initiative list
    InitiativeList list;
    initList(room, &list); // initialises the list
    rollAllInitiatives(&list, room->entities, room->entityCount); // rolls all the entities' initiatives

    // Set the player's position
    //setEntityPosition(room, player, (Position){1,1});

    int running = 1;
    int result = 0;

    // DIALOGUE THINGS

    // printf("room's dialogues\n");

    // printf("room's entering dialogue %s\n", room->roomEnterDialogue);
    // printf("room's starting dialogue %s\n", room->roomStartDialogue);
    // printf("room's clear dialogue %s\n", room->roomClearDialogue);
    // printf("room's exit dialogue %s\n", room->roomExitDialogue);
    // pressAnyKey();

    system("cls");

    LINE;

    // Handle room entering dialogue
    if(!(strcmp(room->roomEnterDialogue, "x") == 0)) {
        char *keywords[] = {"{username}"};
        char *replacements[] = {player->name};
    
        playDialogue(room->roomEnterDialogue, keywords, replacements, 1);
        pressAnyKey();
    }
    else {
        fancyPrint("%s enters %s\n", player->name, room->name);
        pressAnyKey();
    }

    LINE;

    // Handle room starting dialogue
    if(!(strcmp(room->roomStartDialogue, "x")==0)) {
        char *keywords[] = {"{username}"};
        char *replacements[] = {player->name};
    
        playDialogue(room->roomStartDialogue, keywords, replacements, 1);
        pressAnyKey();
    }
    else {
        fancyPrint("%s starts %s\n", player->name, room->name);
        pressAnyKey();
    }

    LINE;

    while(running) {
        // compute fov

        system("cls");
        // determine initiative order
        Entity *currentEntity = getEntityByCurrentInitiative(&list, room->entities, room->entityCount);
        currentEntity->isCurrentTurn=1;

        int entityRunner = runEntity(room,currentEntity,player);

        // run the current entity
        if(entityRunner == 2) {
            // run entity returns 2 when the user choses to finish the room and leave the room
            // leaving room code
            if(!strcmp(room->roomEnterDialogue, "x")) {
                char *keywords[] = {"{username}"};
                char *replacements[] = {player->name};
                playDialogue(room->roomClearDialogue, keywords, replacements, 1);
            }
            fancyPrint("YOU WIN!\n");
            return PLAYER_WIN;
        }

        // change rooms if entity runner returns 10
        if(entityRunner == 10) {
            return 10;
        }

        currentEntity->isCurrentTurn=0;
        cycleInitiative(&list);

        // Check if the player died
        if(player->currentHP<=0) {
            running = 0;
            return PLAYER_LOSE;
        }
    }
    return 0;
}

int runMap(Map *map, Entity *player) {
    int running = 1;

    while(running) {
        RoomGrid *currentRoom = map->currentRoom;
        // Add the player to the entity list
        addEntityToArray(&(currentRoom->entities), &(currentRoom->entityCount), player);
        int roomStatus = runRoom(currentRoom, player);
    
        // Change room!
        if(roomStatus == 10) {
            // get the door
            RoomTile *currentTile = getRoomTileFromGrid(currentRoom, player->gridPos);
            Door *door = currentTile->door;
            //printDoorDetails(door);
            pressAnyKey();
            changeRoom(map, door->targetRoomId, player, door->newPos);
        }
        if(roomStatus == PLAYER_LOSE) {
            return PLAYER_LOSE;
        }
    }
}

int runGame() {
    // Set the random seed!
    srand(time(NULL));

    // Initialise and create the player
    Entity *player = NULL;

    // Initialise a map
    Map *map = createMap();
    loadRoomFolder(map);
    //printRoomsinMap(map);

    // Menu things
    int menuChoice = startGameMenu();

    int mapRunner = 0;

    switch(menuChoice) {
        case 0: {
            exitGame();
            break;
        }
        // NEW GAME MENU
        case 1: {
            player = newGameMenu();

            //printRoomsinMap(map);
            setCurrentRoomId(map, 6);

            mapRunner = runMap(map, player);
        }
    }

    if(mapRunner == PLAYER_LOSE) {
        printGameOver(player);
    }
    if(mapRunner == PLAYER_WIN) {

    }
}

int main() {
    runGame();
    return 0;
}
