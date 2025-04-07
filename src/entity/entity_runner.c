#include "game.h"
#include "entity_runner.h"

// Runs the entity and returns the status 1 - success 0 - failure
int runEntity(RoomGrid *room, Entity *entity, Entity *player) {
    // If the entity is a player, it should run the move menu
    if(entity->type==PLAYER){
        printCurrentEntityTurn(entity);
        return roomActionMenu(room, entity);
    }
    if(entity->type==MONSTER) {
        runMonster(room, entity, player);
    }
}