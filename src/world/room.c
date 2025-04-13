#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#include "../core/game.h"
#include "room.h"
#include "../entity/entity.h"
#include "../util/util.h"

int roomId;

Door *createDoor(Position pos, int targetRoomId, Position newPos);

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
    tile->door = NULL;
    if(symbol == '#') {
        tile->type = WALL;
    }
    else if (symbol == 'D') {
        tile->type = DOOR;
        // DOOR INITIATION HAPPENS LATER!!
    }
    else tile->type = FLOOR;

    tile->visible = 0;
    tile->discovered= 0;
    return tile;
}

RoomTile *getRoomTileFromGrid(RoomGrid *room, Position pos) {
    if(!checkValidPosition) printf("pos out of bounds\n");
    return room->tiles[pos.y][pos.x];
}

// Prints the tile's symbol with a differnet color based on its visiblity
void placeTile(RoomTile *tile) {
    // Check if the tile is visible or discovered
    if (tile->visible) {
        printf("\033[37m %c \033[0m", tile->symbol); // White for visible tiles
    } else if (tile->discovered) {
        printf("\033[90m %c \033[0m", tile->symbol); // Grey for discovered tiles
    } else {
        printf("   ", tile->symbol); // No color for undiscovered tiles (fog)
    }
}

Door *createDoor(Position pos, int targetRoomId, Position newPos) {
    // Allocate memory for a door!!!!
    Door *door = malloc(sizeof(Door));
    if(!door) {
        perror("ERROR - memory allocation for Door failed!");
        return NULL;
    }
    door->roomPos = pos;
    door->targetRoomId = targetRoomId;
    door->newPos = newPos;

    return door;
}

Door **createDoorsArray(int doorCount) {
    Door **doors = malloc(sizeof(Door *) * doorCount);
    if(!doors) {
        perror("Doors array allocation failed");
        return NULL;
    }
    return doors;
}

void printDoorsArray(Door **doors, int doorCount) {
    for(int i = 0; i < doorCount; i++) {
        printf("Door at current room's pos of (%d, %d), leading to %d at (%d, %d)\n",doors[i]->roomPos.x, doors[i]->roomPos.y, doors[i]->targetRoomId, doors[i]->newPos.x, doors[i]->newPos.y);
    }
}

void printDoorDetails(Door *door) {
    printf("Door at current room's pos of (%d, %d), leading to %d at (%d, %d)", door->roomPos.x, door->roomPos.y, door->targetRoomId, door->newPos.x, door->newPos.y);
}

int isOnDoor(RoomGrid *room, Entity *entity) {
    RoomTile *tile = getRoomTileFromGrid(room, entity->gridPos);

    if(tile->door) {
        return 1;
    }
    return 0;
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
    //printf("MAKING %s", roomName);
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
    sscanf(buffer, "ID: %d", &(roomGrid->id));

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

    //printf("buffer is %s\n", buffer);

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
    sscanf(buffer, "START DIALOGUE: %s", roomGrid->roomStartDialogue);

    fgets(buffer, sizeof(buffer), gridFile);
    sscanf(buffer, "CLEAR DIALOGUE: %s", roomGrid->roomClearDialogue);

    fgets(buffer, sizeof(buffer), gridFile);
    sscanf(buffer, "ENTER DIALOGUE: %s", roomGrid->roomEnterDialogue);

    fgets(buffer, sizeof(buffer), gridFile);
    sscanf(buffer, "EXIT DIALOGUE: %s", roomGrid->roomExitDialogue);

    // DOORS!!! connections to different rooms!

    fgets(buffer, sizeof(buffer), gridFile);
    sscanf(buffer, "DOOR COUNT: %d", &(roomGrid->doorCount));

    //printf("DOOR COUNT IS %d\n", roomGrid->doorCount);
    Door **doors = createDoorsArray(roomGrid->doorCount);

    for(int i = 0; i < roomGrid->doorCount; i++) {
        fgets(buffer, sizeof(buffer),  gridFile);
        // Door attributes
        Position roomPos;
        int targetRoomId;
        Position newPos;
        sscanf(buffer, "DOOR: GRID POSITION: %d, %d ; TARGET ROOM ID: %d ; TARGET ROOM POS: %d, %d",
        &(roomPos.x), &(roomPos.y), &targetRoomId, &(newPos.x), &(newPos.y));
        
        // printf("buffer: %s\n", buffer);

        // printf("Door created at %d, %d, which leads to %d (at %d, %d)\n",
        // roomPos.x, roomPos.y, targetRoomId, newPos.x, newPos.y);

        Door *door = createDoor(roomPos, targetRoomId, newPos);
        // printf("Door created at %d, %d, which leads to %d (at %d, %d)\n",
        // door->roomPos.x, door->roomPos.y, door->targetRoomId, door->newPos.x, door->newPos.y);
        // Attach this door to the tile at the position

        RoomTile *tile = getRoomTileFromGrid(roomGrid, roomPos);
        tile->door = door;
        tile->symbol = 'D';

        doors[i] = door;
    }

    roomGrid->doors = doors;
    fclose(gridFile);

    //printDoorsArray(roomGrid->doors, roomGrid->doorCount);

    //pressAnyKey();

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

void printRoomDetails(RoomGrid *room) {
    printf("Room name: %s (id #%d)\n",room->name, room->id);
}