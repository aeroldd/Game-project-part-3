#ifndef GAME_H
#define GAME_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <conio.h>

#define ROOM_PATH "rooms/"
#define ENTITY_PATH "entities/"

#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define MIN(a, b) ((a) < (b) ? (a) : (b))

#define tileSize 5

typedef struct {
    int x, y;
} Position;

#define PLAYER 0
#define MONSTER 1
#define ITEM 2

// game results
#define PLAYER_LOSE 1
#define PLAYER_WIN 2

typedef struct {
    char name[32];
    int type;
    char symbol;
    Position pos;
    Position gridPos;
    int maxHP;
    int currentHP;
    int ac;
    int speed;
    int distanceLeft;
    int initiative;
    int initiativeMod;
    int attack;
    int damage;
    int selected;
    int gold;
    int id;
    int isCurrentTurn;
} Entity;

typedef struct {
    Position pos;
    char symbol;
} RoomTile;

// Initiative linked list storing the index of the entity
typedef struct Node {
    int entityId;  // Index of entity in array
    struct Node *next;
} Node;

typedef struct {
    Node *head;
} InitiativeList;


typedef struct {
    int width;
    int height;
    char name[32];
    Position pos;
    Entity **entities; // dynamically allocated array of pointers to entities
    Entity *player;
    Entity **monsters;
    Entity **items;
    int entityCount;
    int monsterCount;
    RoomTile ***tiles; // 2D array of pointers to RoomTile
    InitiativeList *initiatives; // Pointer to the InitiativeList
} RoomGrid;

typedef struct{
    char* name;
    Position pos;
    char symbol;
    int type;
    int connections[4];
    int discovered;
    int inside;

    char* description;
} Room;


#endif
