#include <stdlib.h>

#include "game.h"
#include "entity_stats.h"

// Update the entity's stats based on the equipment equiped
void updateEntityStats(Entity *entity) {
    // Update the entity's armour
    updateEntityArmour(entity);
    updateEntityWeapon(entity);
}

void updateEntityArmour(Entity *entity) {
    if(entity->armour != NULL) {
        entity->ac = entity->armour->ac;
        return;
    }
    entity->ac = entity->baseAC;
}

void updateEntityWeapon(Entity *entity) {
    if(entity->weapon != NULL) {
        entity->attack = entity->weapon->attack;
        entity->damage = entity->weapon->damage;
        return;
    }
    printf("Entity's base attack: %d, entity's base damage: %d");
    entity->attack = entity->baseAttack;
    entity->damage = entity->baseDamage;   
}

// assumes that the item is already in the player's inventory
void unequipItem(Entity *entity, Item *item) {
    // check if the item is a weapon or an armour piece
    if(!(item->type == WEAPON || item->type == ARMOUR)) return;
    if(item->type == WEAPON) {
        unequipWeapon(entity);
        return;
    }
    unequipArmour(entity);
}

void unequipWeapon(Entity *entity) {
    Weapon *weapon = entity->weapon;
    Item *item = (Item *) weapon;
    // Dialogue keywords and replacements

    // Convert integers to strings - because replacements can only take in strings
    char old_attack_str[10], new_attack_str[10];
    sprintf(old_attack_str, "%d", entity->attack);
    sprintf(new_attack_str, "%d", weapon->attack);

    char old_damage_str[10], new_damage_str[10];
    sprintf(old_damage_str, "%d", entity->damage);
    sprintf(new_damage_str, "%d", weapon->damage);

    char *keywords[] = {"{username}", "{weapon_name}", "{old_attack}", "{new_attack}", "{old_damage}", "{new_damage}"};
    char *replacements[] = {entity->name, item->name, old_attack_str, new_attack_str, old_damage_str, new_damage_str};

    playDialogue("items/unequip_weapon.txt", keywords, replacements, 6);
    
    entity->weapon = NULL;
    updateEntityWeapon(entity);
}

void unequipArmour(Entity *entity) {
    Armour *armour = entity->armour;
    Item *item = (Item*) armour;

    // Dialogue keywords and replacements

    // Convert integers to strings - because replacements can only take in strings
    char old_ac_str[10], new_ac_str[10];
    sprintf(old_ac_str, "%d", entity->ac);
    sprintf(new_ac_str, "%d", armour->ac);
    char *keywords[] = {"{username}", "{armour_name}", "{old_ac}", "{new_ac}"};
    char *replacements[] = {entity->name, item->name, old_ac_str, new_ac_str};

    playDialogue("items/unequip_armour.txt", keywords, replacements, 4);

    entity->armour = NULL;
    updateEntityArmour(entity);
}