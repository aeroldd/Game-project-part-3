#include <stdlib.h>
#include <stdio.h>

#include "../core/game.h"
#include "monster.h"
#include "../dialogue/dialogue.h"
#include "pathfinding.h"
#include "../gameplay/combat.h"
#include "../gameplay/initiative.h"
#include "../util/util.h"
#include "../world/room.h"
#include "../world/camera.h"
#include "../entity/entity.h"

// Runs the monster AI
void runMonster(RoomGrid *room, Entity *entity, Entity *player) {
    // check how close the player is, and according to it it will move towards the player
    int distanceFromPlayer = getDistancePos(entity->gridPos, player->gridPos);
    if(distanceFromPlayer < 30) {
        printCurrentEntityTurn(entity);
        int pathLength = 10;
        Position *path = findShortestPath(room, entity->gridPos, player->gridPos, &pathLength);

        // Iterate through the path array and move the monster to the position
        //printf("moving to the player\n");
        for(int i = 1; (i < pathLength-1) && (entity->distanceLeft >= 5); i++) {
            //printf("step %d -> %d, %d\n", i, path[i].x, path[i].y);
            Position oldPos = entity->gridPos;
            if(setEntityPosition(room, entity, path[i])) {
                entity->distanceLeft -=5;
                //printf("My distance left is %d", entity->distanceLeft);
            }
            // Reaches the else statement when the entity has colided with a wall or an entity
            else{
                // collide w something
            }
            displayRoomWithPlayerCamera(room, entity, 5);
            delay(1000);
            system("cls");
        }

        // check if the entity has reached the player
        Entity **entities = getEntitiesAroundPoint(room, entity->gridPos);
        int entityCount = 0, inAttackRange = 0;
        if (entities) {
            for (int i = 0; entities[i] != NULL; i++) {
                if (entities[i]->type == PLAYER) {
                    inAttackRange = 1;
                    entityCount++;
                }
            }
        }

        // hit a random player that is in attack range (if they are in attack range)
        if(inAttackRange) {
            //printf("IN ATTACK RANGE IMMA HIT YOU!\n");
            int target = rand() % entityCount;
            //printf("target at index %d\n", target);
            attack(room, entity, entities[target]);
        }

        // Reset their distance left
        entity->distanceLeft = entity->speed;
    }
    // If the player isnt in the detection range, then it will just randomly move
    else {
        printf("====================\n");
        playDialogue("monsters_still_in_room.txt", NULL, NULL, 0);
        printf("\n====================\n");
        monsterRoam(room, entity, entity->speed);
        pressAnyKey();
    }
}

// Move the monster randomly in the room when the player isnt in attack range
void monsterRoam(RoomGrid *room, Entity *entity, int maxDistance) {
    if (!room || !entity) return; // Safety check

    int distance = rand() % (maxDistance / 5); // Random step size
    int direction = rand() % 4; // 0 = up, 1 = down, 2 = left, 3 = right

    Position newPos = entity->gridPos; // Start from current position

    // iterate through the distances

    for(int i = 0; i < distance ; i += 5) {
        switch (direction) {
            case 0: newPos.y -= 1; break; // Move up
            case 1: newPos.y += 1; break; // Move down
            case 2: newPos.x -= 1; break; // Move left
            case 3: newPos.x += 1; break; // Move right
        }
    }

    // Update entity position
    setEntityPosition(room, entity, newPos);
}
