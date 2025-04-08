#ifndef ITEM_H
#define ITEM_H

#include "../core/game.h"

// Creates an item from a text file
Item *createItemFromFile(char* itemName, int itemType);

int useItem(Entity *entity, Item *item);

int equipWeapon(Entity *entity, Weapon *weapon);
int equipArmour(Entity *entity, Armour *armour);

// Prints the item's details
void printItemDetails(Item *item);

void deleteItem(Entity *entity, Item *item);

#endif