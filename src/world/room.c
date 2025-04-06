#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "../core/game.h"
#include "room.h"
#include "../entity/entity.h"
#include "../util/util.h"


RoomTile ***createRoomTilesArray(RoomGrid *room) {
    RoomTile ***roomTiles = malloc(room->height * sizeof(RoomTile **));
    if (!roomTiles) {
        perror("ERROR - memory allocation for room tiles columns failed.");
        return NULL;
    }
    for (int i = 0; i < room->height; i++) {
        roomTiles[i] = malloc(room->width * sizeof(RoomTile *));
        if (!roomTiles[i]) {
            perror("ERROR - memory allocation for room tiles rows failed.");
            return NULL;
        }
    }
    return roomTiles;
}

RoomTile *createRoomTile(Position pos, char symbol) {
    RoomTile *tile = malloc(sizeof(RoomTile));
    if (!tile) {
        perror("ERROR - memory allocation for RoomTile failed");
        return NULL;
    }
    tile->pos = pos;
    tile->symbol = symbol;
    if(symbol == '#') {
        tile->type = WALL;
    }
    else tile->type = FLOOR;
    return tile;
}

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
            if (found) continue;
            printf(" %c ", room->tiles[y][x]->symbol);
        }
        printf(" |\n");
    }
    for (int x = xMin; x <= xMax; x++) {
        printf("---");
    }
    printf("-\n");
}

int getDistancePos(Position pos1, Position pos2) {
    return (int) tileSize * abs(sqrt(pow((pos2.x - pos1.x), 2) + pow((pos2.y - pos1.y), 2)));
}

int checkValidPosition(RoomGrid room, Position pos) {
    if((pos.x > 0) && (pos.x < room.width) && (pos.y > 0) && (pos.y < room.height))
        return 1;
    return 0;
}

RoomGrid *createRoomGrid(char* roomName) {
    char filePath[64];
    strcpy(filePath, ROOM_PATH);
    strcat(filePath, roomName);
    FILE *gridFile = fopen(filePath, "r");
    if(!gridFile) {
        printf("ERROR - The room you have entered doesn't exist.");
        return NULL;
    }
    char buffer[1024] = "";
    RoomGrid *roomGrid = malloc(sizeof(RoomGrid));
    if (!roomGrid) {
        perror("ERROR - memory allocation for room failed");
        fclose(gridFile);
        return NULL;
    }
    fgets(buffer, sizeof(buffer), gridFile);
    sscanf(buffer, "ROOM NAME: %s", roomGrid->name);
    fgets(buffer, sizeof(buffer), gridFile);
    sscanf(buffer, "DIMENSIONS: %d, %d", &(roomGrid->width), &(roomGrid->height));
    roomGrid->tiles = createRoomTilesArray(roomGrid);
    for (int y = 0; y < roomGrid->height; y++) {
        fgets(buffer, sizeof(buffer), gridFile);
        for (int x = 0; x < roomGrid->width; x++) {
            roomGrid->tiles[y][x] = createRoomTile((Position){x,y}, buffer[x]);
        }
    }

    // get the entity count from the file
    int entityCount;
    fgets(buffer, sizeof(buffer), gridFile);
    sscanf(buffer, "ENTITY COUNT: %d", &entityCount);

    roomGrid->entities = createEntitiesArray();
    roomGrid->entityCount = 0;

    // monster counter
    roomGrid->monsterCount = 0;
    for (int i = 0; i < entityCount; i++) {
        fgets(buffer, sizeof(buffer), gridFile);
        char monsterFile[1024];
        Position monsterGridPos;
        int currentHP;
        sscanf(buffer, "MONSTER FILE: %s ; GRID POSITION: %d,%d ; CURRENT HP: %d",
               monsterFile, &monsterGridPos.x, &monsterGridPos.y, &currentHP);
        Entity *monster = createEntityFromFile(monsterFile, roomGrid->pos, monsterGridPos, currentHP);
        int temp;
        addEntityToArray(&roomGrid->entities, &(roomGrid->entityCount), monster);
        roomGrid->monsterCount++; // Increase the monster count
    }

    // set the start and finish dialogues

    fgets(buffer, sizeof(buffer), gridFile);
    sscanf(buffer, "START DIALOGUE: %s", roomGrid->roomEnterDialogue);

    fgets(buffer, sizeof(buffer), gridFile);
    sscanf(buffer, "CLEAR DIALOGUE: %s", roomGrid->roomClearDialogue);

    // set the 
    fclose(gridFile);
    return roomGrid;
}

Entity **getEntitiesAroundPoint(RoomGrid *room, Position targetPos) {
    Entity **entities = malloc(5 * sizeof(Entity *));
    if (!entities) {
        perror("ERROR - Memory allocation failed.\n");
        return NULL;
    }
    int entitiesFound = 0;
    int horizontal[4] = {0, 0, 1, -1};
    int vertical[4] = {1, -1, 0, 0};
    for (int i = 0; i < 4; i++) {
        Position pos = {targetPos.x + horizontal[i], targetPos.y + vertical[i]};
        Entity *e = getEntityAtGridPosition(room, pos);
        if (e) {
            entities[entitiesFound++] = e;
        }
    }
    entities[entitiesFound] = NULL;
    if (entitiesFound == 0) {
        free(entities);
        return NULL;
    }
    entities = realloc(entities, (entitiesFound + 1) * sizeof(Entity *));
    return entities;
}

// Check if the room is cleared!
// Rooms are cleared when there are no more monsters in the room
int isRoomCleared(RoomGrid *room) {
    //printf("There are %d monsters in the room", room->monsterCount);
    return (room->monsterCount == 0);
}

void printRoomCleared(RoomGrid *room, Entity *player) {
    fancyPrint("After %s's tough battles...\n", player->name);
    delay(500);
    printf("================================\n");
    fancyPrint("Room cleared!!!\n");
    printf("================================\n");
}