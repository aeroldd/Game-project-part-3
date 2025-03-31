#include "game.h"
#include "entity.h"
#include "room.h"
#include "combat.h"
#include "util.h"
#include "dialogue.h"
#include "pathfinding.h"

void printGameOver(Entity *player);

int runRoom(char* roomName, Entity *player) {
    // Create the room
    RoomGrid *room = createRoomGrid(roomName);

    printf("room successfully made!");

    //printEntities(room->entities, room->entityCount);

    // Add the player to the entity list
    addEntityToArray(&(room->entities), &(room->entityCount), player);

    // Initialises the initiative list
    InitiativeList list;
    initList(room, &list); // initialises the list
    rollAllInitiatives(&list, room->entities, room->entityCount); // rolls all the entities' initiatives

    // Set the player's position
    setEntityPosition(room, player, (Position){5,5});

    int running = 1;
    int result = 0;

    while(running) {
        system("cls");
        // determine initiative order
        Entity *currentEntity = getEntityByCurrentInitiative(&list, room->entities, room->entityCount);
        currentEntity->isCurrentTurn=1;

        //printEntityStats(currentEntity);

        // run the current entity
        runEntity(room, currentEntity, player);

        currentEntity->isCurrentTurn=0;
        cycleInitiative(&list);

        // Check if the player died
        if(player->currentHP<=0) {
            running = 0;
            result = PLAYER_LOSE;
        }

        if(isRoomCleared(room)) {
            running = 0;
            result = PLAYER_WIN;
        }
    }

    if(result == PLAYER_LOSE) {
        printGameOver(player);
    }
    if(result == PLAYER_WIN) {
        printRoomCleared(room, player);
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
    Entity *player;
    createPlayer(&player);

    playDialogue("dungeon_enter.txt");

    runRoom("room_2.txt", player);
    return 0;
}
