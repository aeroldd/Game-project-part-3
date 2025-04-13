#include "camera.h"
#include "game.h"
#include "room.h"

#include <stdio.h>

void displayRoom(RoomGrid *room) {
    printf("reaches");
    for (int y = 0; y < room->height; y++) {
        for (int x = 0; x < room->width; x++) {
            printf("%c", room->tiles[y][x]->symbol);
        }
        printf("\n");
    }
}

void displayRoomWithPlayerCamera(RoomGrid *room, Entity *p, int camSize) {
    //printEntities(room->entities, room->entityCount);

    int xMin = MAX(p->gridPos.x - camSize, 0);
    int xMax = MIN(p->gridPos.x + 1 + camSize, room->width);
    int yMin = MAX(p->gridPos.y - camSize, 0);
    int yMax = MIN(p->gridPos.y + 1 + camSize, room->height);
    for (int x = xMin; x <= xMax; x++) {
        printf("---");
    }
    printf("-\n");
    for (int y = yMin; y < yMax; y++) {
        printf("| ");

        for (int x = xMin; x < xMax; x++) {
            int found = 0;
            for (int z = 0; z < room->entityCount; z++) {
                Entity *currentEntity = room->entities[z];
                if(room->tiles[y][x]->visible) {

                    if ((currentEntity->gridPos.x == x) && (currentEntity->gridPos.y == y)) {
    
                        // Highlight the entity with yellow if the entity is selected
                        if (currentEntity->selected)
                            printf("\033[33m %c \033[0m", room->entities[z]->symbol);
    
                        // Highlight the entity with cyan if its the entity's 
                        else if (currentEntity->isCurrentTurn) {
                            printf("\033[36m %c \033[0m", room->entities[z]->symbol);
                        }
                        // Check the type of the entity
                        // If the entity is a player mark it with blue, if the entity is an enemy, mark it with red
                        else {
                            if (currentEntity->type == MONSTER) {
                                printf("\033[31m %c \033[0m", currentEntity->symbol);
                            }
                            else if (currentEntity->type == PLAYER) {
                                printf("\033[34m %c \033[0m", currentEntity->symbol);
                            }
                        }
                            //printf(" %c ", room->entities[z]->symbol);
                        found = 1;
                        break;
                    }
                }
            }
            if (found) continue;

            // FOG OF WAR VISIBILITY!!
            placeTile(room->tiles[y][x]);
        }
        printf(" |\n");
    }
    for (int x = xMin; x <= xMax; x++) {
        printf("---");
    }
    printf("-\n");
}

// NEW DISPLAYING CAMERA FUNCTION USING CURSES???

