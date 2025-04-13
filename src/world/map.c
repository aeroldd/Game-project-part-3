#include <stdlib.h>
#include <stdio.h>
#include <dirent.h> // directory stream library to parse directories for rooms later on

#include "../core/game.h"
#include "map.h"
#include "room.h"
#include "../entity/entity.h"
#include "../util/util.h"

// forward declarations
RoomGrid **createRoomArray();
void addToRoomArray(RoomGrid ***rooms, int *roomCount, RoomGrid *room);

Map *createMap() {
    Map *map = malloc(sizeof(Map));
    if(!map) { // check if it was successfully made
        perror("Failed allocation for map.\n"); 
        return NULL;
    }

    // create a room array and allocate it to the map
    RoomGrid **rooms = createRoomArray();
    map->rooms = rooms;

    map->roomCount = 0;

    return map;
}

RoomGrid **createRoomArray() {
    RoomGrid **rooms = malloc(sizeof(RoomGrid *));
    if(!rooms) {
        perror("Failed allocation for room array in map.");
        return NULL;
    }
    return rooms;    
}

void addToRoomArray(RoomGrid ***rooms, int *roomCount, RoomGrid *room) {
    // validating rooms and room
    if (!rooms) {
        perror("Room array doesn't exist\n");
        return;
    }
    if (!room) {
        perror("Room doesn't exist\n");
        return;
    }

    // reallocate memory

    (*roomCount)++; //incrementing the room count
    RoomGrid **newRooms = realloc(*rooms, (*roomCount) * sizeof(RoomGrid *));
    
    (*rooms) = newRooms;

    // add the room to the end of the array
    (*rooms)[(*roomCount) - 1] = room;
}

void addRoomToMap(Map *map, RoomGrid *room) {
    addToRoomArray(&(map->rooms), &(map->roomCount), room);
}

void setCurrentRoom(Map *map, RoomGrid *room) {
    map->currentRoom = room;
}

void setCurrentRoomId(Map *map, int id) {
    setCurrentRoom(map, getRoomById(map, id));
}

void printRoomsinMap(Map *map) {
    for(int i = 0; i < map->roomCount; i++) {
        printf("Room (#%d) name is this one ->%s\n", map->rooms[i]->id, map->rooms[i]->name);
    }
}

RoomGrid *getRoomById(Map *map, int roomId) {
    int roomCount = map->roomCount;
    RoomGrid **rooms = map->rooms;
    for(int i = 0; i < roomCount; i++) {
        if(rooms[i]->id == roomId) return rooms[i];
    }
    printf("Room with id %d not found\n");
    return NULL;
}

void changeRoom(Map *map, int roomId, Entity *entity, Position newPos) {
    setCurrentRoomId(map, roomId);
    // place the player in the room
    RoomGrid *room = getRoomById(map, roomId);
    //addEntityToArray(&(room->entities), &(room->entityCount), entity);
    entity->gridPos = newPos;
}

// parses through the rooms directory and loads all the rooms in
void loadRoomFolder(Map *map) {
    DIR *dir = opendir(ROOM_PATH); // opens the rooms directory
    if (!dir) {
        perror("Could not open rooms directory");
        return;
    }

    struct dirent *entry;
    char fullPath[256];

    // iterate through all the files in the room directory
    while ((entry = readdir(dir)) != NULL) {
        // skip . and ..
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // check if it's a .txt file
        if (strstr(entry->d_name, ".txt")) {
            // construct full path
            snprintf(fullPath, sizeof(fullPath), "%s/%s", ROOM_PATH, entry->d_name);

            // load room
            RoomGrid *room = createRoomGrid(entry->d_name) ;
            if (room) {
                addRoomToMap(map, room);
                printf("Loaded room from: %s\n", fullPath);
            } else {
                fprintf(stderr, "Failed to load room: %s\n", fullPath);
            }
        }
    }

    closedir(dir);
}