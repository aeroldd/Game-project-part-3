#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include "../core/game.h"
#include "entity.h"
#include "../gameplay/menus.h"
#include "../gameplay/combat.h"
#include "../dialogue/dialogue.h"
#include "../gameplay/initiative.h"
#include "monster.h"

// Implement entity-related functions

int entityId;

Entity *createEntity(char *name, int type, char symbol, Position mapPos, Position gridPos,
                     int maxHP, int ac, int speed, int initiativeMod, int attack, int damage, int gold, int viewRadius, int blind) {
    Entity *entity = malloc(sizeof(Entity));
    if (!entity) return NULL;
    strncpy(entity->name, name, sizeof(entity->name) - 1);
    entity->name[sizeof(entity->name) - 1] = '\0';
    entity->type = type;
    entity->symbol = symbol;

    entity->pos = mapPos;
    entity->gridPos = gridPos;

    entity->maxHP = maxHP;
    entity->currentHP = maxHP;

    entity->ac = ac;
    entity->baseAC = ac;

    entity->speed = speed;
    entity->initiativeMod = initiativeMod;
    entity->distanceLeft = speed;

    entity->attack = attack;
    entity->baseAttack = attack;

    entity->damage = damage;
    entity->baseDamage = damage;

    entity->selected = 0;
    entity->gold = gold;

    entity->initiative=0;
    entity->isCurrentTurn=0;

    entity->detectionRadius = viewRadius;
    entity->blind = blind;

    entity->inventory = NULL;
    entity->armour = NULL;
    entity->weapon = NULL;

    return entity;
}

Entity *createEntityFromFile(char* entityName, Position mapPos, Position gridPos, int currentHealth) {
    // Prepare the file path of the entity
    char filePath[64];
    strcpy(filePath, ENTITY_PATH);
    strcat(filePath, entityName);

    // Open the file
    FILE *entityFile = fopen(filePath, "r");
    if (!entityFile) {
        printf("ERROR - The entity %s you have entered doesn't exist.\n", filePath);
        return NULL;
    }

    // Allocate memory for the single entity
    Entity *entity = malloc(sizeof(Entity));
    if(!entity) {
        perror("Error - memory allocation for entity unsuccessful.");
        fclose(entityFile);
        return NULL;
    }


    char buffer[1024] = "";

    fgets(buffer, sizeof(buffer), entityFile);
    sscanf(buffer, "NAME: %[^\n]", entity->name);

    fgets(buffer, sizeof(buffer), entityFile);
    sscanf(buffer, "TYPE: %d", &(entity->type));

    fgets(buffer, sizeof(buffer), entityFile);
    sscanf(buffer, "SYMBOL: %c", &(entity->symbol));

    entity->pos = mapPos;
    entity->gridPos = gridPos;

    fgets(buffer, sizeof(buffer), entityFile);
    sscanf(buffer, "MAX HEALTH: %d", &(entity->maxHP));

    // The entity's current health is set by the function - for default health (same health max), the value -1 can be
    // passed as an arguement
    if(currentHealth == -1) entity->currentHP = entity->maxHP;
    else entity->currentHP = currentHealth;

    fgets(buffer, sizeof(buffer), entityFile);
    sscanf(buffer, "ARMOUR: %d", &(entity->ac));

    fgets(buffer, sizeof(buffer), entityFile);
    sscanf(buffer, "SPEED: %d", &(entity->speed));

    fgets(buffer, sizeof(buffer), entityFile);
    sscanf(buffer, "INITIATIVE MOD: %d", &(entity->initiativeMod));

    fgets(buffer, sizeof(buffer), entityFile);
    sscanf(buffer, "ATTACK: %d", &(entity->attack));

    fgets(buffer, sizeof(buffer), entityFile);
    sscanf(buffer, "DAMAGE: %d", &(entity->damage));

    fgets(buffer, sizeof(buffer), entityFile);
    sscanf(buffer, "GOLD REWARD: %d", &(entity->gold));

    entity->selected = 0;
    entity->initiative = 0;
    entity->isCurrentTurn = 0;
    entity->distanceLeft = entity->speed;

    entity->baseAttack = entity->attack;
    entity->baseDamage = entity->damage;
    entity->baseAC = entity->ac;
    entity->baseSpeed = entity->speed;

    entity->weapon=NULL;

    fclose(entityFile);
    return entity;
}

void setEntityName(Entity *entity, const char* name) {
    strncpy(entity->name, name, sizeof(entity->name) - 1); // Copy safely
    entity->name[sizeof(entity->name) - 1] = '\0'; // Null-terminate to avoid overflow
}

Entity *getEntityAtGridPosition(RoomGrid *room, Position pos) {
    for (int i = 0; i < room->entityCount; i++) {
        if ((room->entities[i]->gridPos.x == pos.x) && (room->entities[i]->gridPos.y == pos.y)) {
            return room->entities[i];
        }
    }
    return NULL;
}

int setEntityPosition(RoomGrid *room, Entity *entity, Position pos) {
    if (pos.x >= 0 && pos.x < room->width && pos.y >= 0 && pos.y < room->height) {
        if(room->tiles[pos.y][pos.x]->symbol == '#') {
            //printf("that is a wall!\n");
            return 0;
        } else {
            if(getEntityAtGridPosition(room, pos)) {
                //printf("there is an entity on this tile\n");
                return 0;
            }
            entity->gridPos = pos;
            return 1;
        }
    }
    return 0;
}

void removeEntityById(Entity ***entities, int *count, int targetId) {
    int found = 0;
    for (int i = 0; i < *count; i++) {
        if ((*entities)[i]->id == targetId) {
            // Free the entity
            free((*entities)[i]);
            found = 1;

            // Shift remaining entities left
            for (int j = i; j < *count - 1; j++) {
                (*entities)[j] = (*entities)[j + 1];
            }

            // Shrink array size
            (*count)--;
            *entities = realloc(*entities, (*count) * sizeof(Entity *));
            if (*count > 0 && *entities == NULL) {
                perror("Memory reallocation failed");
                exit(1);
            }

            //printf("Entity with ID %d removed.\n", targetId);
            return;
        }
    }
    if (!found) {
        //printf("Entity with ID %d not found.\n", targetId);
    }
}

Entity *getEntityById(Entity **entities, int entityCount, int entityId) {
    for(int i = 0; i < entityCount; i++) {
        if(entities[i]->id == entityId) return entities[i];
    }
    perror("Error - entity with id not found.");
    return NULL;
}

Entity **createEntitiesArray() {
    Entity **entities = malloc(sizeof(Entity *));
    if (entities) {
        entities[0] = NULL;
        return entities;
    }
    perror("ERROR - memory allocation for entities array failed.\n");
    return NULL;
}

int getEntityCount(Entity **entities) {
    if (!entities) return 0;
    int count = 0;
    while (entities[count] != NULL) {
        count++;
    }
    return count;
}

void addEntityToArray(Entity ***entitiesList, int *entityCount, Entity *entity) {
    entity->id=entityId;

    entityId++;
    if (*entitiesList == NULL) {
        *entitiesList = malloc(2 * sizeof(Entity *));
        if (!*entitiesList) {
            perror("failed initial allocation");
            return;
        }
    } else {
        Entity **newList = realloc(*entitiesList, (*entityCount + 2) * sizeof(Entity *));
        if (!newList) {
            perror("failed memory allocation");
            return;
        }
        *entitiesList = newList;
    }
    (*entitiesList)[*entityCount] = entity;
    (*entitiesList)[*entityCount + 1] = NULL;
    (*entityCount)++;

    //printf("id %d added. entity count is: %d\n", entityId, *entityCount);
}

void printEntities(Entity **entities, int entityCount){
    printf("entity count -> %d\n", entityCount);
    for(int i = 0; i < entityCount; i++) {
        printf("i reach here!\n");
        printf("%d) %s (id #%d) (Type: %d) at position %d, %d, character symbol: %c\n", i, entities[i]->name, entities[i]->id, entities[i]->type, entities[i]->gridPos.x, entities[i]->gridPos.y, entities[i]->symbol);
    }
    printf("\n");
}

// Print the entity's stats
void printEntityStats(Entity *entity) {
    printf("====================\n");
    printf("%s's stats\n", entity->name);
    printf("====================\n");
    printf("HP: %d/%d\t", entity->currentHP, entity->maxHP);
    printf("Armour: %d\t", entity->ac);
    printf("Travel distance left: %d (%d speed)\n", entity->distanceLeft, entity->speed);
    printf("Gold: %d\t", entity->gold);

    char* weaponName;
    // No weapon
    if(!entity->weapon) weaponName = "Fists";
    else weaponName = entity->weapon->base.name;

    printf("Weapon: %s (+%d hit, %d)\n", weaponName, entity->attack, entity->damage);
    printf("====================\n");
    printf("Grid position: %d, %d\n", entity->gridPos.x, entity->gridPos.y);
    printf("====================\n");
}

