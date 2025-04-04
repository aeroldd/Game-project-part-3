#include <stdlib.h>
#include <stdio.h>

#include "combat.h"
#include "entity.h"
#include "util.h"
#include "initiative.h"

int damageEntity(Entity *e, int damage) {
    //printf("damaged %s\n", e->name);
    e->currentHP -= damage;
    return damage;
}

int attackRoll(Entity *e) {
    int r = rolld20();
    if (r == 100 || r == 101) return r;
    return r + e->attack;
}

int calculateHit(int attack, int ac) {
    return (attack > ac) ? 1 : 0;
}

int attack(RoomGrid *room, Entity *attacker, Entity *reciever) {
    int rollVal = attackRoll(attacker);
    int hit = calculateHit(rollVal, reciever->ac);

    system("cls");
    printAttackMessage(attacker, reciever);

    delay(1000);
    
    if (hit) {
        int damage = damageEntity(reciever, attacker->damage);
        printAttackSuccess(attacker, reciever, damage);
    } else {
        printAttackFailure(attacker, reciever);
    }

    delay(1000);

    if (reciever->currentHP <= 0) {
        printKillMessage(attacker, reciever);
        kill(room, attacker, reciever);
    } else {
        printHealthLeft(reciever);
    }

    delay(1000);

    return hit;
}

void printAttackMessage(Entity *attacker, Entity *reciever) {
    printf("==========\n");
    fancyPrint("%s tries to attack %s!\n", attacker->name, reciever->name);
    printf("==========\n");
}

void printAttackSuccess(Entity *attacker, Entity *reciever, int damage) {
    // TODO: ADD TEXT FILE DIALOGUES FOR EACH ENTITY!
    fancyPrint("%s hits %s, dealing %d damage!\n", attacker->name, reciever->name, damage);
}

void printAttackFailure(Entity *attacker, Entity *reciever) {
    // TODO: ADD TEXT FILE DIALOGUES FOR EACH ENTITY!
    fancyPrint("%s misses their attack!\n", attacker->name);
}

void printKillMessage(Entity *attacker, Entity *reciever) {
    // TODO: ADD DEATH AND KILL MESSAGES FOR EACH ENTITY!
    fancyPrint("%s kills %s! %s lies on the floor lifeless.\n", attacker->name, reciever->name, reciever->name);
}

void printHealthLeft(Entity *entity) {
    // TODO: Different states (such as wounded, nothing, barely scratched for each level of health) for each entity in text files
    fancyPrint("%s has %d HP left.\n", entity->name, entity->currentHP);

    int healthPercentage = (entity->currentHP * 100) / entity->maxHP;

    if(healthPercentage > 80) {
        fancyPrint("%s is barely scratched!\n", entity->name);
    }
    else if(healthPercentage > 60) {
        fancyPrint("%s is lightly wounded!\n", entity->name);
    }
    else if(healthPercentage > 40) {
        fancyPrint("%s is moderately wounded!\n", entity->name);
    }
    else if(healthPercentage > 20) {
        fancyPrint("%s is severely wounded!\n", entity->name);
    }
    else if(healthPercentage > 0) {
        fancyPrint("%s pleads for mercy!\n", entity->name);
    }
}

void kill(RoomGrid *room, Entity *attacker, Entity *reciever) {
    int id = reciever->id;
    if (reciever->type == MONSTER) {
        //printf("%s has died!\n", reciever->name);
        reward(attacker, reciever->gold);
    }
    removeEntityById(&(room->entities), &(room->entityCount), id);
    InitiativeList *list = room->initiatives;
    removeEntityInitiative(list, id);
    room->monsterCount -= 1;
}

void reward(Entity *e, int gold) {
    e->gold += gold;
    fancyPrint("%s gained %d gold!\n", e->name, gold);
    delay(1000);
}
