#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    char name[50];
    int initiative;
} Entity;

typedef struct Node {
    int entityIndex;  // Index of entity in array
    struct Node *next;
} Node;

typedef struct {
    Node *head;
} InitiativeList;

// Insert index into linked list in initiative order
void insertInOrder(InitiativeList *list, int entityIndex, Entity *entities) {
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->entityIndex = entityIndex;
    newNode->next = NULL;

    if (!list->head || entities[entityIndex].initiative > entities[list->head->entityIndex].initiative) {
        newNode->next = list->head;
        list->head = newNode;
        return;
    }

    Node *current = list->head;
    while (current->next && entities[entityIndex].initiative <= entities[current->next->entityIndex].initiative) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;
}

// Print initiative order using entity array
void printInitiativeOrder(InitiativeList *list, Entity *entities) {
    printf("\nInitiative Order:\n");
    Node *current = list->head;
    while (current) {
        printf("%s (Initiative: %d)\n", entities[current->entityIndex].name, entities[current->entityIndex].initiative);
        current = current->next;
    }
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

// Example usage
int main() {
    InitiativeList list = { NULL };

    // Store entities in an array
    Entity entities[4] = {
        {"Goblin", 15},
        {"Orc", 12},
        {"Wizard", 18},
        {"Knight", 14}
    };

    // Insert entity indices into initiative list
    insertInOrder(&list, 0, entities);
    insertInOrder(&list, 1, entities);
    insertInOrder(&list, 2, entities);
    insertInOrder(&list, 3, entities);

    printInitiativeOrder(&list, entities);
    freeList(&list);

    return 0;
}