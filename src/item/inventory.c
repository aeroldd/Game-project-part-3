#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../core/game.h"
#include "inventory.h"
#include "item.h"
#include "../util/util.h"

void initInventory(Inventory **inventory, int maxCapacity) {
    *inventory = malloc(sizeof(Inventory));
    if(!*inventory) {
        printf("Error - memory allocation for inventory failed!\n");
        free(*inventory);
        *inventory = NULL;
        return;
    }

    (*inventory)->items = malloc(sizeof(Item *) * maxCapacity);
    if(!(*inventory)->items) {
        printf("Error - memory allocation for inventory items failed!\n");
        free((*inventory)->items);
        *inventory = NULL;
        return;
    }

    (*inventory)->itemCount=0;
    (*inventory)->maxCapacity= maxCapacity;
    (*inventory)->owner=NULL;
    (*inventory)->selected=0;
    (*inventory)->selectedIndex=0;
}

void printInventoryDetails(Inventory *inventory) {
    printf("reaches");
    for(int i = 0; i < inventory->itemCount; i++) {
        printf("Item %d: \n", i);
        printItemDetails(inventory->items[i]);
        printf("\n");
    }
}

int addItemToInventory(Inventory *inventory, Item *item) {
    // Check if the item is a consumable - consumables can be stacked
    if(item->type == CONSUMABLE) {
        // Check if the consumable is in the inventory
        for(int i = 0; i < inventory->itemCount; i++) {
            if(strcmp(inventory->items[i]->name, item->name) && (inventory->items[i]->type == CONSUMABLE)) {
                // consumable found!
                // cast the item and the item in the inventory as a consumable
                Consumable *consumable = (Consumable *)inventory->items[i];
                Consumable *newConsumable = (Consumable *) item;

                // increase the consumable in the inventory item count
                consumable->count += newConsumable->count;
                
                return 1;
            }
        }
    }

    // Check if the inventory still has space
    if (inventory->itemCount >= inventory->maxCapacity) {
        printf("Inventory is full. Failed adding the item to the inventory.\n");
        return 0;
    }

    // Check if the item is null
    if(!item) {
        printf("Item is not initialized properly.\n");
        return 0;
    }

    // Add the item to the inventory
    inventory->items[inventory->itemCount] = item;
    (inventory->itemCount)++;

    return 1;
}

Item *getItemFromInventoryIndex(Inventory *inventory, int itemIndex) {
    Item *item = inventory->items[itemIndex];
    if(!item) return NULL;
    return item;
}

void addItemToEntityInventory(Entity *entity, Item *item) {
    // Check if the entity even has an inventory
    if(!entity->inventory) {
        printf("Error - the entity %s doesnt even have an inventory to store this item in\n", entity->name);
        return;
    }

    if(!addItemToInventory(entity->inventory, item)){
        printf("failed!");
        return;
    }
    fancyPrint("%s picked up %s! %s stores it in their inventory.\n", entity->name, item->name, entity->name);
}

void attachInventoryToEntity(Entity *entity, int maxCapacity) {
    Inventory *inventory;
    initInventory(&inventory, maxCapacity);
    entity->inventory = inventory; 
    inventory->owner = entity;
}

// Prints the entity's inventory bar
void printInventoryBar(Entity *player, int selected, int selectedItem) {
    Inventory *inventory = player->inventory;

    // if the inventory is selected, the background and foreground colors will be inverted
    if(selected) printf("\x1b[7m > %s's inventory < \x1b[0m ", player->name);     
    else printf("%s's inventory ", player->name);
    printf("|");

    // Iterate through the inventory's items and print out the names
    for(int i = 0; i < inventory->itemCount; i++) {
        // if the item is selected, the background and foreground colors will be inverted
        if(i == selectedItem && selected) {
            printf("\x1b[7m > %d - %s < \x1b[0m", i, inventory->items[i]->name);
        }
        else {
            printf(" %d - %s ", i, inventory->items[i]->name);
        }
        printf("|");
    }
    printf("\n");
}