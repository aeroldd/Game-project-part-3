#include "../core/game.h"

#include "entity_runner.h"
#include "monster.h"
#include "player.h"
#include "../gameplay/menus.h"
#include "../gameplay/initiative.h"

// Runs the entity and returns the status 1 - success 0 - failure
int runEntity(RoomGrid *room, Entity *entity, Entity *player) {
    // If the entity is a player, it should run the move menu
    if(entity->type==PLAYER){
        if(room->initiatives->length!=1) {
            printCurrentEntityTurn(entity);
        }
        return roomActionMenu(room, entity);
    }
    if(entity->type==MONSTER) {
        runMonster(room, entity, player);
    }
}