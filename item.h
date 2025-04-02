#ifndef ITEM_H
#define ITEM_H

#include "game.h"

// Creates an item from a text file
Item *createItemFromFile(char* itemName, int itemType);

int useItem(Entity *entity, Item *item);

int equipWeapon(Entity *entity, Weapon *weapon);
int equipArmour(Entity *entity, Armour *armour);

// Prints the item's details
void printItemDetails(Item *item);

// Initialises an inventory
void initInventory(Inventory **inventory, int maxCapacity);

// Prints all the contents of an inventory
void printInventoryDetails(Inventory *inventory);

// Adds an item to an inventory, returns 1 if success, else 0
int addItemToInventory(Inventory *inventory, Item *item);

Item *getItemFromInventoryIndex(Inventory *inventory, int itemIndex);

// Adds an item to an entity's inventory
void addItemToEntityInventory(Entity *entity, Item *item);

// Creates an inventory for an entity
void attachInventoryToEntity(Entity *entity, int maxCapacity);

// Print the inventory bar in the bottom of the screen
void printInventoryBar(Entity *entity, int selected, int selectedItem);

#endif