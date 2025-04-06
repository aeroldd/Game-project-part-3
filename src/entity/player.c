#include <stdlib.h>

#include "player.h"
#include "entity.h"
#include "../util/util.h"
#include "../world/room.h"
#include "../item/item.h"

// Player creation
Entity *createPlayerEntity(char *name, char symbol, Position mapPos, Position gridPos) {
    Entity *player = createEntityFromFile("player_default.txt", mapPos, gridPos, -1);

    setEntityName(player, name);
    player->symbol = symbol;
    player->weapon = NULL;

    // Initialise the player's inventory
    initInventory(&(player->inventory), 4);

    return player;
}

// Player movement handling - moves the player depending on the key pressed
int movePlayer(RoomGrid *room, Entity *player, char key) {
    //Position oldPos = player->gridPos;
    Position newPos;

    // Check if the player still has movement left
    if(player->distanceLeft != 0) {
        // Find the new positions based on the key the user pressed
        switch(key) {
            case 'w': newPos = (Position){player->gridPos.x, player->gridPos.y - 1}; break;
            case 's': newPos = (Position){player->gridPos.x, player->gridPos.y + 1}; break;
            case 'a': newPos = (Position){player->gridPos.x - 1, player->gridPos.y}; break;
            case 'd': newPos = (Position){player->gridPos.x + 1, player->gridPos.y}; break;
        }
    
        if (setEntityPosition(room, player, newPos)) { // the set entity position returns 1 when there are no collisions
            // if the room isnt cleared, movement is limited by distance left
            if(!isRoomCleared(room)) {
                player->distanceLeft -= 5;
                if (player->distanceLeft == 0) fancyPrint("You have expended all movements!\n");
                return 1;
            }
            player->distanceLeft = player->speed;
    
            return 1;
        }
    }

    if(player->distanceLeft == 0) {
        fancyPrint("You have run out of movement!!\n");
        return 0;
    }

    // collides with a wall
    return 2;
}