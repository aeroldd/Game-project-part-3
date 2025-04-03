#include "game.h"
#include "item.h"
#include "util.h"
#include "dialogue.h"
#include "entity.h"

Item *createItemFromFile(char* itemName, int itemType) {
    // Prepare the file path of the item
    char filePath[128];
    strcpy(filePath, ITEM_PATH);

    char typePath[64];
    switch(itemType) {
        case WEAPON: {
            strcpy(typePath, "weapons/");
            break;
        }
        case ARMOUR: {
            strcpy(typePath, "armour/");
            break;
        }
        case CONSUMABLE: {
            strcpy(typePath, "consumables/");
            break;
        }
    }

    strcat(filePath, typePath);
    strcat(filePath, itemName);

    //printf("filePath is %s", filePath);

    // Open the file
    FILE *itemFile = fopen(filePath, "r");
    if (!itemFile) {
        printf("ERROR - The item %s you have entered doesn't exist.\n", filePath);
        return NULL;
    }

    // Allocate memory for the item depending on the type of item

    // Allocate a null item pointer
    Item *item = NULL;

    char buffer[1024] = "";

    switch(itemType) {
        case WEAPON:{
            // allocate memory for a weapon
            Weapon *weapon = malloc(sizeof(Weapon));

            if (!weapon) {
                printf("Memory allocation for weapon failed!\n");
                fclose(itemFile);
                return NULL;
            }

            item = (Item*)weapon;
            
            // Go through the file and do things yay!
            
            fgets(buffer, sizeof(buffer), itemFile);
            sscanf(buffer, "NAME: %[^\n]", item->name);
            
            fgets(buffer, sizeof(buffer), itemFile);
            sscanf(buffer, "ATTACK: %d", &(weapon->attack));
            
            fgets(buffer, sizeof(buffer), itemFile);
            sscanf(buffer, "DAMAGE: %d", &(weapon->damage));
            
            break;
        }

        case ARMOUR:{
            // allocate memory for a weapon
            Armour *armour = malloc(sizeof(Armour));

            if (!armour) {
                printf("Memory allocation for weapon failed!\n");
                fclose(itemFile);
                return NULL;
            }

            item = (Item*)armour;
            
            // Go through the file 
            
            fgets(buffer, sizeof(buffer), itemFile);
            sscanf(buffer, "NAME: %[^\n]", item->name);
            
            fgets(buffer, sizeof(buffer), itemFile);
            sscanf(buffer, "AC: %d", &(armour->ac));            
            break;
        }

        case CONSUMABLE: {
            // allocate memory for a weapon
            Consumable *consumable = malloc(sizeof(Consumable));

            if (!consumable) {
                printf("Memory allocation for weapon failed!\n");
                fclose(itemFile);
                return NULL;
            }

            item = (Item*)consumable;
            
            // Go through the file
            
            fgets(buffer, sizeof(buffer), itemFile);
            sscanf(buffer, "NAME: %[^\n]", item->name);
            
            fgets(buffer, sizeof(buffer), itemFile);
            sscanf(buffer, "VALUE: %d", &(consumable->value));            
            break;
        }
    }
    
    item->type=itemType;

    fclose(itemFile);
    //printf("successfully made the item!\n");
    return item;
}

int useItem(Entity *entity, Item *item) {
    // determine the item's type
    int type = item->type;
    switch(type) {
        case WEAPON: {
            // Cast the item back to a weapon
            Weapon *weapon = (Weapon*) item;
            equipWeapon(entity, weapon);
            return 1;
        }
        case ARMOUR: {
            // Cast the item as armour
            Armour *armour = (Armour*) armour;
            equipArmour(entity, armour);
            return 1;
        }
        case CONSUMABLE: {
            return 1;
        }
    }
    return 0;
}

// deletes an item from an entity's inventory
int deleteItemFromInventory(Entity *entity, Item *item, int count) {
    Inventory *inventory = entity->inventory;

    // terminate if the item doesnt even exist
    if(!item) return 0;

    // Iterate through the inventory and check if the item is there
    for(int i = 0; i < inventory->itemCount; i++) {
        // strcmp is used because strings in c is a pointer to an array of characters so its weird and funny
        if((strcmp(inventory->items[i]->name, item->name)) == 0) {
            // Check if the item is a consumable
            if(item->type == CONSUMABLE) {
                // reduce the count of the consumable by the count
                Consumable *consumable = (Consumable *) inventory->items[i];
                consumable->count -= count;

                // if the consumable's count is 0 or less, the item is deleted
                if(consumable->count <= 0) {
                    deleteItem(entity, item);
                }
                return 1;
            }

            // Every other item that isnt a consumable
            deleteItem(entity, item);
            inventory->itemCount--;
            return 1;
        }
    }
    printf("Couldn't find %s in inventory.\n", item->name);
    return 0;
}

// Removes an item from the inventory and frees it
void deleteItem(Entity *entity, Item *item) {
    if (!entity || !item) return;

    Inventory *inventory = entity->inventory;

    // Unequip the item if it's equipped
    unequipItem(entity, item);

    // Find the item in the inventory
    int found = 0;
    for (int i = 0; i < inventory->itemCount; i++) {
        if (inventory->items[i] == item) {
            found = 1;
            // Shift items left to remove the item
            for (int j = i; j < inventory->itemCount - 1; j++) {
                inventory->items[j] = inventory->items[j + 1];
            }
            inventory->items[inventory->itemCount - 1] = NULL;  // Clear last slot
            inventory->itemCount--;  // Reduce count
            break;
        }
    }

    if (!found) {
        printf("Item not found in inventory!\n");
        return;
    }

    // Free the memory
    free(item);
    printf("Item deleted successfully.\n");
}

void printItemDetails(Item *item) {
    // Check the item's type
    printf("Item's name: %s\n", item->name);

    switch(item->type) {
        case WEAPON: {
            Weapon *weapon = (Weapon*)item;
            printf("%s's type: Weapon\n", item->name);
            printf("%s's attack: %d\n", item->name, weapon->attack);
            printf("%s's damage: %d\n", item->name, weapon->damage);
            break;
        }
        case ARMOUR: {
            Armour *armour = (Armour*)item;
            printf("%s's type: Armour\n", item->name);
            printf("%s's armour class: %d\n", item->name, armour->ac);
            break;
        }
        case CONSUMABLE: {
            Consumable *consumable = (Consumable*)item;
            printf("%s's type: Consumable\n", item->name);
            printf("%s's value: %d\n", item->name, consumable->value);
            break;
        }
    }
}

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
        if(i == selectedItem) {
            printf("\x1b[7m > %d - %s < \x1b[0m", i, inventory->items[i]->name);
        }
        else {
            printf(" %d - %s ", i, inventory->items[i]->name);
        }
        printf("|");
    }
    printf("\n");
}

// Equip armour
int equipArmour(Entity *player, Armour *armour) {
    Item *item = (Item*) armour;

    // Dialogue keywords and replacements

    // Convert integers to strings - because replacements can only take in strings
    char old_ac_str[10], new_ac_str[10];
    sprintf(old_ac_str, "%d", player->ac);
    sprintf(new_ac_str, "%d", armour->ac);
    char *keywords[] = {"{username}", "{armour_name}", "{old_ac}", "{new_ac}"};
    char *replacements[] = {player->name, item->name, old_ac_str, new_ac_str};

    player->armour = armour;
    playDialogue("items/equip_armour.txt", keywords, replacements, 4);
    updateEntityArmour(player);

    return 1;
}

// Equip weapon
int equipWeapon(Entity *player, Weapon *weapon) {
    Item *item = (Item*) weapon;

    // Dialogue keywords and replacements

    // Convert integers to strings - because replacements can only take in strings
    char old_attack_str[10], new_attack_str[10];
    sprintf(old_attack_str, "%d", player->attack);
    sprintf(new_attack_str, "%d", weapon->attack);

    char old_damage_str[10], new_damage_str[10];
    sprintf(old_damage_str, "%d", player->damage);
    sprintf(new_damage_str, "%d", weapon->damage);

    char *keywords[] = {"{username}", "{weapon_name}", "{old_attack}", "{new_attack}", "{old_damage}", "{new_damage}"};
    char *replacements[] = {player->name, item->name, old_attack_str, new_attack_str, old_damage_str, new_damage_str};

    player->weapon = weapon;
    playDialogue("items/equip_weapon.txt", keywords, replacements, 6);
    updateEntityWeapon(player);

    return 1;
}

// Set an entity's armour and upate their stats
int setArmour(Entity *entity, Armour *armour) {
    entity->armour = armour;
    updateEntityArmour(entity);
}

// Set an entity's weapon and update their stats
int setWeapon(Entity *entity, Weapon *weapon) {
    entity->weapon = weapon;
    updateEntityWeapon(entity);
}

// int main() {
//     printf("Hello world\n");
//     // create an item

//     Inventory *inventory = NULL;
//     initInventory(&inventory, 10);

//     Item *i1 = createItemFromFile("rusty_sword.txt", WEAPON);
//     Item *i2 = createItemFromFile("short_sword.txt", WEAPON);

//     Item *i3 = createItemFromFile("apple.txt", CONSUMABLE);
//     Item *i4 = createItemFromFile("small_potion.txt", CONSUMABLE);

//     Item *i5 = createItemFromFile("leather_armour.txt", ARMOUR);

//     addItemToInventory(inventory, i1);
//     addItemToInventory(inventory, i2);
//     addItemToInventory(inventory, i3);
//     addItemToInventory(inventory, i4);
//     addItemToInventory(inventory, i5);

//     printInventoryDetails(inventory);


//     return 0;
// }