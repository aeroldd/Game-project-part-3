#include <stdlib.h>
#include <stdio.h>

#include "../core/game.h"
#include "map.h"
#include "room.h"

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
