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

#define LINE printf("====================\n")

typedef struct {
    int x, y;
} Position;

#define PLAYER 0
#define MONSTER 1
#define ITEM 2

// game results
#define PLAYER_LOSE 1
#define PLAYER_WIN 2

// ITEMS

// ITEM TYPES

#define ITEM_PATH "items/"

#define WEAPON 0
#define ARMOUR 1
#define CONSUMABLE 2

typedef struct {
    char name[32];
    int type;
} Item;

typedef struct {
    Item **items;
    int itemCount;
    int maxCapacity;
} Inventory;

typedef struct {
    Item base;
    int attack;
    int damage;
    int equipped;
} Weapon;

typedef struct {
    Item base;
    int ac;
    int equipped;
} Armour;

typedef struct {
    Item base;
    int value;
    int count;
} Consumable;

// Entity things

typedef struct {
    char name[32];
    int type;
    char symbol;
    Position pos;
    Position gridPos;
    int maxHP;
    int currentHP;

    int baseAC;
    int ac;

    int baseSpeed;
    int speed;

    int distanceLeft;
    int initiative;
    int initiativeMod;

    int baseAttack;
    int attack;

    int baseDamage;
    int damage;

    int selected;
    int gold;
    int id;
    int isCurrentTurn;

    int detectionRadius;
    int blind;

    Inventory *inventory;
    Armour *armour;
    Weapon *weapon;
} Entity;

#define FLOOR 0
#define WALL 1

typedef struct {
    Position pos;
    char symbol;
    int type;
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

    // dialogue file that plays before and after playing a room
    char roomEnterDialogue[128];
    char roomClearDialogue[128];
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
