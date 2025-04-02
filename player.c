#include "game.h"
#include "player.h"

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
                if (player->distanceLeft == 0) printf("You have expended all your movement!\n");
                return 1;
            }
            player->distanceLeft = player->speed;
    
            return 1;
        }
    }

    fancyPrint("You have run out of movement!!\n");
    return 0;
}