#include "room.h"
#include "entity.h"
#include "combat.h"
#include "util.h"

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
                if ((room->entities[z]->gridPos.x == x) && (room->entities[z]->gridPos.y == y)) {
                    if (room->entities[z]->selected)
                        printf("\033[33m %c \033[0m", room->entities[z]->symbol);
                    else if (room->entities[z]->isCurrentTurn) {
                        printf("\033[36m %c \033[0m", room->entities[z]->symbol);
                    }
                    else
                        printf(" %c ", room->entities[z]->symbol);
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

void displayMoveMenu() {
    printf("==========\n");
    printf("MOVE\n");
    printf("==========\n");
    printf("[W A S D to move] [x to cancel]\n");
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

    // set the 
    fclose(gridFile);
    return roomGrid;
}

int moveMenu(RoomGrid *room, Entity *p) {
    int runningMoveMenu = 1, confirmedMovement = 0;
    char input;
    int distanceLeft = p->distanceLeft;
    Position oldPos = p->gridPos, newPos = oldPos;
    displayMoveMenu();
    displayRoomWithPlayerCamera(room, p, 5);
    do {
        int distance = getDistancePos(oldPos, newPos);
        p->distanceLeft = distanceLeft;
        system("cls");
        printf("Moving!!!\nDistance: %d  Remaining: %d\n[WASD to move, x to cancel, c to confirm]\n", distance, distanceLeft);
        displayRoomWithPlayerCamera(room, p, 5);
        printf("Position: (%d, %d)\n", newPos.x, newPos.y);
        input = getch();
        switch(input) {
            case 'w': newPos = (Position){p->gridPos.x, p->gridPos.y - 1}; break;
            case 's': newPos = (Position){p->gridPos.x, p->gridPos.y + 1}; break;
            case 'a': newPos = (Position){p->gridPos.x - 1, p->gridPos.y}; break;
            case 'd': newPos = (Position){p->gridPos.x + 1, p->gridPos.y}; break;
            case 'x': newPos = oldPos; distanceLeft = p->speed; runningMoveMenu = 0; confirmedMovement = 1; break;
            case 'c': runningMoveMenu = 0; confirmedMovement = 1; break;
            default: break;
        }
        if (setEntityPosition(room, p, newPos)) {
            distance = getDistancePos(oldPos, newPos);
            distanceLeft = p->distanceLeft- distance;
            if (distanceLeft >= 5)
                continue;
            printf("You have expended all your movement!\n");
            displayRoomWithPlayerCamera(room, p, 5);
            p->distanceLeft = 0;
            runningMoveMenu = 0;
        }
    } while (runningMoveMenu);

    if (!confirmedMovement) {
        printf("Confirm movement? [c for yes] >> ");
        input = getch();
        if (input != 'c') {
            printf("Movement cancelled. Reverting position.\n");
            setEntityPosition(room, p, oldPos);
        }
    }
    return 1;
}

int roomActionMenu(RoomGrid *room, Entity *player) {
    // The room action menu will keep running until the distance left is 0 and one attack has been done
    int runningRoomActionMenu = 1;
    int attacked = 0;
    while(runningRoomActionMenu) {
        system("cls");
        printEntityStats(player);
        displayRoomWithPlayerCamera(room, player, 5);
        printf("ACTIONS\n");
        printf("==========\n");

        if(player->distanceLeft != 0) {
            printf("MOVE: m (distance left: %d)\n", player->distanceLeft);
        }

        Entity **entities = getEntitiesAroundPoint(room, player->gridPos);
        int entityCount = 0, inAttackRange = 0;
        if (entities) {
            for (int i = 0; entities[i] != NULL; i++) {
                if (entities[i]->type == MONSTER) {
                    inAttackRange = 1;
                    entityCount++;
                }
            }
        }
        if (inAttackRange && !attacked) {
            printf("ATTACK: a\n");
        }
    
        printf("SKIP: x\n");
        char key = getch();
        switch(key) {
            case 'm': {
                if(player->distanceLeft == 0) {
                    printf("You have already used up all your movement!\n");
                }
                else moveMenu(room, player);
                break;
            }
            case 'a': {
                if(!attacked) {
                    attacked = attackMenu(room, player, entities, entityCount);
                }
                else printf("You already used your attack!\n");
                break;
            }
            // skips the turn when x is pressed
            case 'x': {
                printf("Skipping %s's turn.", player->name);
                runningRoomActionMenu=0;
                break;
            }
            default: {
                perror("ERROR - wrong key pressed.");
                return 1;
            }
        }

        if(player->distanceLeft == 0 && attacked == 1) {
            runningRoomActionMenu = 0;
        }

    }
    // Reset the entity's distance left
    player->distanceLeft = player->speed;
    return 0;
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
    printf("There are %d monsters in the room", room->monsterCount);
    return (room->monsterCount == 0);
}

void printRoomCleared(RoomGrid *room, Entity *player) {
    fancyPrint("After %s's tough battles...", player->name);
    delay(500);
    printf("================================\n");
    fancyPrint("Room cleared!!!\n");
    printf("================================\n");
}