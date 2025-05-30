#ifndef INVENTORY_H
#define INVENTORY_H

#include "../core/game.h"

// Initialises an inventory
void initInventory(Inventory **inventory, int maxCapacity);

// Prints all the contents of an inventory
void printInventoryDetails(Inventory *inventory);

// Adds an item to an inventory, returns 1 if success, else 0
int addItemToInventory(Inventory *inventory, Item *item);

// Item deletion
int deleteItemFromInventory(Entity *entity, Item *item, int count);
void deleteItem(Entity *entity, Item *item);

Item *getItemFromInventoryIndex(Inventory *inventory, int itemIndex);

// Adds an item to an entity's inventory
void addItemToEntityInventory(Entity *entity, Item *item);

// Creates an inventory for an entity
void attachInventoryToEntity(Entity *entity, int maxCapacity);

// Print the inventory bar in the bottom of the screen
void printInventoryBar(Entity *entity, int selected, int selectedItem);

#endif