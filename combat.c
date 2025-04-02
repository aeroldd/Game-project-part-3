#include "combat.h"
#include "entity.h"
#include "util.h"

int damageEntity(Entity *e, int damage) {
    //printf("damaged %s\n", e->name);
    e->currentHP -= damage;
    return damage;
}

int roll(int max) {
    return (rand() % max) + 1;
}

int rolld20() {
    int r = roll(20);
    if (r == 20) return 100;  // critical
    if (r == 1) return 101;   // critical failure
    return r;
}

int attackRoll(Entity *e) {
    int r = rolld20();
    if (r == 100 || r == 101) return r;
    return r + e->attack;
}

int calculateHit(int attack, int ac) {
    return (attack > ac) ? 1 : 0;
}

int attack(RoomGrid *room, Entity *attacker, Entity *reciever) {
    int rollVal = attackRoll(attacker);
    int hit = calculateHit(rollVal, reciever->ac);

    system("cls");
    printAttackMessage(attacker, reciever);

    delay(1000);
    
    if (hit) {
        int damage = damageEntity(reciever, attacker->damage);
        printAttackSuccess(attacker, reciever, damage);
    } else {
        printAttackFailure(attacker, reciever);
    }

    delay(1000);

    if (reciever->currentHP <= 0) {
        printKillMessage(attacker, reciever);
        kill(room, attacker, reciever);
    } else {
        printHealthLeft(reciever);
    }

    delay(1000);

    return hit;
}

void printAttackMessage(Entity *attacker, Entity *reciever) {
    printf("==========\n");
    fancyPrint("%s tries to attack %s!\n", attacker->name, reciever->name);
    printf("==========\n");
}

void printAttackSuccess(Entity *attacker, Entity *reciever, int damage) {
    // TODO: ADD TEXT FILE DIALOGUES FOR EACH ENTITY!
    fancyPrint("%s hits %s, dealing %d damage!\n", attacker->name, reciever->name, damage);
}

void printAttackFailure(Entity *attacker, Entity *reciever) {
    // TODO: ADD TEXT FILE DIALOGUES FOR EACH ENTITY!
    fancyPrint("%s misses their attack!\n", attacker->name);
}

void printKillMessage(Entity *attacker, Entity *reciever) {
    // TODO: ADD DEATH AND KILL MESSAGES FOR EACH ENTITY!
    fancyPrint("%s kills %s! %s lies on the floor lifeless.\n", attacker->name, reciever->name, reciever->name);
}

void printHealthLeft(Entity *entity) {
    // TODO: Different states (such as wounded, nothing, barely scratched for each level of health) for each entity in text files
    fancyPrint("%s has %d HP left.\n", entity->name, entity->currentHP);

    int healthPercentage = (entity->currentHP * 100) / entity->maxHP;

    if(healthPercentage > 80) {
        fancyPrint("%s is barely scratched!\n", entity->name);
    }
    else if(healthPercentage > 60) {
        fancyPrint("%s is lightly wounded!\n", entity->name);
    }
    else if(healthPercentage > 40) {
        fancyPrint("%s is moderately wounded!\n", entity->name);
    }
    else if(healthPercentage > 20) {
        fancyPrint("%s is severely wounded!\n", entity->name);
    }
    else if(healthPercentage > 0) {
        fancyPrint("%s pleads for mercy!\n", entity->name);
    }
}

void kill(RoomGrid *room, Entity *attacker, Entity *reciever) {
    int id = reciever->id;
    if (reciever->type == MONSTER) {
        //printf("%s has died!\n", reciever->name);
        reward(attacker, reciever->gold);
    }
    removeEntityById(&(room->entities), &(room->entityCount), id);
    InitiativeList *list = room->initiatives;
    removeEntityInitiative(list, id);
    room->monsterCount -= 1;
}

void reward(Entity *e, int gold) {
    e->gold += gold;
    fancyPrint("%s gained %d gold!\n", e->name, gold);
    delay(1000);
}

void initList(RoomGrid *room, InitiativeList *list) {
    list->head = NULL;
    room->initiatives=list;
}

void rollInitiative(Entity *e) {
    //printf("initiative before rolling %d, initiative modifier %d\n", e->initiative, e->initiativeMod);
    e->initiative = roll(20) + e->initiativeMod;
    //printf("Initiative for %s rolled -> %d\n", e->name, e->initiative);
}

void rollAllInitiatives(InitiativeList *initiativeList, Entity **entities, int entityCount) {
    //printf("Determining entity initiatives\n");
    for(int i = 0; i < entityCount; i++) {
        rollInitiative(entities[i]);
        insertInOrder(initiativeList, entities, entityCount, entities[i]->id);
    }
    //printf("rolled all initiatives!\n");
}

// Insert index into linked list in initiative order
void insertInOrder(InitiativeList *list, Entity **entities, int entityCount, int entityId) {
    // allocate memory to create a new node
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->entityId = entityId;
    newNode->next = NULL;

    //printf("reaches");

    // If the list is empty, it will set the first item of the list to the new node
    if (!list->head) {
        newNode->next = newNode; // Points to itself
        list->head = newNode;
        return;
    }

    Node *current = list->head, *prev = NULL;
    
    Entity *entity = getEntityById(entities, entityCount, entityId);

    // Case 1: Insert before head (if new entity has the highest initiative)
    if (entity->initiative > getEntityById(entities, entityCount, list->head->entityId)->initiative) {
        // Find the last node (the one pointing to the head)
        while (current->next != list->head)
            current = current->next;

        // Insert new node at the front
        newNode->next = list->head;
        list->head = newNode;
        current->next = list->head;  // Maintain circularity
        return;
    }

    // Case 2: Find the correct insertion point
    do {
        prev = current;
        current = current->next;
    } while (current != list->head && entity->initiative <= getEntityById(entities, entityCount, current->entityId)->initiative);

    // Insert the new node in its sorted position
    prev->next = newNode;
    newNode->next = current;
}

// Removes entity from the initiative list
void removeEntityInitiative(InitiativeList *list, int entityId) {
    if (!list->head) return;  // If list is empty, do nothing

    Node *current = list->head, *prev = NULL;

    // Case 1: If the head is the one to remove
    if (current->entityId == entityId) {
        if (current->next == list->head) {  // Only one node in the list
            free(current);
            list->head = NULL;
            return;
        }

        // Find the last node (which points to the head)
        while (current->next != list->head)
            current = current->next;

        // Adjust pointers
        Node *toDelete = list->head;
        list->head = list->head->next;  // Move head to next element
        current->next = list->head;     // Last node now points to new head
        free(toDelete);
        return;
    }

    // Case 2: Removing a non-head node
    do {
        prev = current;
        current = current->next;
    } while (current != list->head && current->entityId != entityId);

    if (current == list->head) return;  // Not found, exit

    prev->next = current->next;
    free(current);
}

void cycleInitiative(InitiativeList *list){
    Node *current = list->head, *prev = NULL;
    list->head = current->next;
}

Entity *getEntityByCurrentInitiative(InitiativeList *list, Entity **entities, int entityCount) {
    Entity *currentEntity = getEntityById(entities, entityCount, list->head->entityId);
    //printf("It is %s's turn. (Initiative is %d)\n", currentEntity->name, currentEntity->initiative);
    return currentEntity;
}

// Print initiative order using entity array
void printInitiativeOrder(InitiativeList *list, Entity **entities, int entityCount) {
    printf("\nInitiative Order:\n");
    Node *current = list->head;
    do {
        Entity *currentEntity = getEntityById(entities, entityCount, current->entityId);
        printf("%s (Initiative: %d)\n", currentEntity->name, currentEntity->initiative);
        current = current->next;
    } while(current!=list->head);
}

// Free the list
void freeList(InitiativeList *list) {
    Node *current = list->head;
    while (current) {
        Node *temp = current;
        current = current->next;
        free(temp);
    }
    list->head = NULL;
}

void printCurrentEntityTurn(Entity *entity) {
    fancyPrint("==========\n");
    fancyPrint("%s's turn\n", entity->name);
    fancyPrint("==========\n");
    delay(1000);
    //system("cls");
}
