#include "game.h"
#include "entity.h"
#include "room.h"
#include "item.h"
#include "util.h"
#include "combat.h"
#include "menus.h"
#include "input.h"
#include "dialogue.h"
#include "player.h"

int startGameMenu() {
    printGameTitle();
    printf("New Game [1]\n");
    printf("Load Game [2]\n");
    printf("How to play [3]\n");
    printf("Exit game [0]\n"); 

    char key = getKeyPress();
    switch(key) {
        case '1': {
            // when there is a new game, a new player is made!
            return 1;
            break;
        }
        case '2': {
            // loading previous saved games
            // TODO: add saving 💔💔💔😭😭
            return 2;
            break;
        }
        case '3': {
            // how to play menu!
            return 3;
            break;
        }
        case '0': {
            // exit the game!
            return 0;
            break;
        }
        default: {
            system("cls");
            startGameMenu();
        }
    }
    return 0;
}

Entity *newGameMenu() {
    system("cls");
    printGameTitle();
    printf("NEW GAME\n");
    Entity *player = NULL;
    createPlayerMenu(&player);

    // NEW GAME THINGS

    char *dialogueKeywords[] = {"{username}"};
    char *dialogueReplacements[] = {player->name};
    playDialogue("dungeon_enter.txt", dialogueKeywords, dialogueReplacements, 1);

    // create a rusty sword object and give it to the player

    Item *rusty_sword = createItemFromFile("rusty_sword.txt", WEAPON);
    addItemToEntityInventory(player, rusty_sword);

    return player;
}

// menu for creating a player
void createPlayerMenu(Entity **player) {
    char username[32];
    char symbol;
    LINE;
    fancyPrint("MAKE YOUR CHARACTER\n");
    LINE;
    fancyPrint("Enter your username >> ");
    scanf("%s", &username);
    
    // Clear newline from the buffer
    while (getchar() != '\n');

    fancyPrint("Enter your player's map symbol (only one character please) >> ");
    symbol = getchar();
    
    *player = createPlayerEntity(username, symbol, (Position) {2,2}, (Position) {2,2});
    printf("================================\n");
}

int loadGameMenu() {
    printf("Load previous save files!\n");
}

void howToPlayMenu() {
    printf("game tutorial!\n");
}

void exitGame() {
    printf("====================\n");
    fancyPrint("Thank you for playing my game!\n");
    printf("====================\n");
    delay(3000);
    system("exit");
}

void printGameTitle() {
    printf("====================\n");
    printf("DUNGEON.txt\n");
    printf("====================\n");
}

int roomActionMenu(RoomGrid *room, Entity *player) {
    // Reset the player's distance left
    player->distanceLeft = player->speed;

    // The room action menu will keep running until the distance left is 0 and one attack has been done
    int runningRoomActionMenu = 1;
    int attacked = 0;

    int inventorySelected = 0;
    int selectedItemIndex = 0;

    while(runningRoomActionMenu) {
        system("cls");
        printEntityStats(player);
        displayRoomWithPlayerCamera(room, player, 5);
        printf("====================\n");
        printInventoryBar(player, inventorySelected, selectedItemIndex);
        printf("====================\n");
        printf("ACTIONS\n");
        printf("====================\n");

        if(player->distanceLeft != 0) {
            printf("MOVE: WASD (distance left: %d)\n", player->distanceLeft);
        }
        
        if(!isRoomCleared(room)) {
            if(player->distanceLeft == 0 && attacked) {
                printf("SKIP TURN (all actions have been used) [0]");
            }
            else printf("SKIP TURN [0]\n");
        }
        else printf("FINISH ROOM: [0]\n");

        // Checking if the player can attack (There are monsters in range)
        Entity **entities = getEntitiesAroundPoint(room, player->gridPos);
        int entityCount = 0, inAttackRange = 0;
        if (entities) {
            for (int i = 0; entities[i] != NULL; i++) {
                if (entities[i]->type == MONSTER) {
                    inAttackRange = 1;
                    entityCount++;
                }
            }
        }

        if(!inventorySelected) {
            if(inAttackRange && !attacked) {
                printf("ATTACK: [1]\n");
            }
            else if(attacked && inAttackRange) {
                printf("ALREADY USED UP ALL ATTACKS.\n");
            }
        }

        if(!inventorySelected) printf("INVENTORY: [2]\n");

        
        // Inventory actions
        // Get the current selected item
        Item *currentItem = getItemFromInventoryIndex(player->inventory, selectedItemIndex);
        if(inventorySelected) {
            if(currentItem->type == WEAPON || currentItem->type == ARMOUR) {
                // if the item is equipped, unequip item, else equip item
                if(((player->weapon == NULL)  && (currentItem->type == WEAPON)) || ((player->armour == NULL) && (currentItem->type == ARMOUR))) {
                    printf("EQUIP %s: [5]\n", currentItem->name);
                }
                else printf("UNEQUIP %s: [5]\n", currentItem->name);
            }
            else printf("CONSUME %s: [5]\n", currentItem->name);
            printf("DROP %s: [6]\n", currentItem->name);
            printf("EXIT INVENTORY MODE: [x]\n");
        }

        char key = getKeyPress();
    
        switch(key) {
            case 'w':
            case 'a':
            case 's':
            case 'd':
            {
                movePlayer(room, player, key);
                break;
            }
            // The user skips the player's turn
            case '0': {
                // if the room isnt cleared and 0 is pressed, the turn is skipped
                if(!isRoomCleared(room)) {
                    fancyPrint("Skipping %s's turn.\n", player->name);
                    runningRoomActionMenu = 0;
                    break;
                }

                // if the room is cleared and 0 is pressed, the room is completed!
                return 2;

                break;
            }
            case '1': {
                // If inventory mode isnt selected, number 1 will attack enemies
                if(!inventorySelected) {
                    // Check if there are monsters in range
                    if(!inAttackRange) {
                        fancyPrint("There are no monsters in range.\n");
                        continue;
                    }
                    // Check if the player has already used their attack
                    if(!attacked) {
                        attacked = attackMenu(room, player, entities, entityCount);
                        continue;
                    }
                    printf("You already used your attack!\n");
                    break;
                }

                // Inventory mode selected!
                // Set the selected item to 0
                selectedItemIndex = 0;

                break;
            }
            case '2': {
                // Inventory!!
                if(!inventorySelected) {
                    inventorySelected = 1;
                    break;
                }

                // Inventory mode is selected!
                // Set the selected item to 1

                selectedItemIndex = 1;

                break;
            }

            case '3': {
                if(inventorySelected) {
                    selectedItemIndex = 2;
                }
                break;
            }

            case '4': {
                if(inventorySelected) {
                    selectedItemIndex = 3;
                }
                break;
            }

            // use item
            case '5': {
                if(!inventorySelected) break;
                // Use the item at the current selected item index
                if(!currentItem) {
                    printf("There is no item at index %d", selectedItemIndex);
                    break;
                }
                useItem(player, currentItem);
                break;
            }

            // drop item
            case '6': {
                if (!inventorySelected) break;
                // Drop the item at the current selected item index
                deleteItemFromInventory(player, currentItem, 1);
                //unequipItem(player, currentItem);
            }

            // read item description
            case '7': {
                if (!inventorySelected) break;
                // Read the item's description at the selected item index
            }

            case 'x': {
                inventorySelected=0;
            }

        }
    }
    system("cls");
    printEntityStats(player);
    displayRoomWithPlayerCamera(room, player, 5);
    printf("====================\n");
    fancyPrint("%s's turn has ended!\n", player->name);
    printf("====================\n");
    delay(500);
    pressAnyKey();
    return 1;
}

// Attacking menu!
/*
This will:
-clear the window
-show the screen with the entities and the selected target
-attack the selected target
*/
int attackMenu(RoomGrid *room, Entity *p, Entity **entities, int entityCount) {
    int runningAttackMenu = 1, isTargetChosen = 0, target = 0;
    do {
        system("cls");
        printf("==========\nATTACK MENU\n%s's HP: %d\n", p->name, p->currentHP);
        entities[target]->selected = 1;
        printf("====================\n");
        displayRoomWithPlayerCamera(room, p, 3);
        printf("====================\n");
        printf("ACTIONS:\n");
        printf("CHANGE TARGET [spacebar]\n");
        printf("CONFIRM TARGET [1]\n");
        printf("CANCEL ATTACK [x]\n");
        printf("====================\n");
        printf("CURRENT TARGET: %s at (%d, %d)\n", entities[target]->name,entities[target]->gridPos.x, entities[target]->gridPos.y);

        char key = getKeyPress();
        switch(key) {
            case ' ':
                entities[target]->selected = 0;
                target = (target == entityCount - 1) ? 0 : target + 1;
                break;
            case '1':
                isTargetChosen = 1;
                runningAttackMenu = 0;
                break;
            case 'x':
                runningAttackMenu = 0;
                break;
        }

    } while(runningAttackMenu);
    if (isTargetChosen) {
        attack(room, p, entities[target]);
        entities[target]->selected = 0;
        return 1;
    }
    return 0;
}

// The inventory menu!
void inventoryMenu(int *selected) {
    *selected = 1;
}