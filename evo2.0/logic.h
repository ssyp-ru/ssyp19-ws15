#ifndef _10_07_2019_LOGIC
#define _10_07_2019_LOGIC

#include "player.h"
#include "game.h"

int logic_min_to_max(const void* val1, const void* val2);

int logic_max_to_min(const void* val1, const void* val2);

uint8_t logic_create_new_animal(player_t* player);

animal_t* logic_get_neightbour(player_t* player, uint16_t animal_num, uint8_t num);

uint8_t logic_use_double_mods(player_t* player);

uint8_t logic_use_symbiosis(player_t* player);

uint8_t logic_protect_animal(players_t* players, animal_t* animal, uint16_t player_num);

uint8_t logic_use_hibernation_ability(player_t* player);

void logic_use_other_card(player_t* player, uint8_t num);

uint8_t logic_use_card(players_t* players, dist_t* dist, uint16_t player_num);

uint8_t logic_add_mods(players_t* players, dist_t* dist, uint16_t player_num);

uint8_t get_first_state(uint16_t num);

uint8_t get_second_state(uint16_t num);

uint8_t logic_add_parasite_to_animal(players_t* players, dist_t* dist, uint16_t player_num, uint8_t weight);

void logic_use_piracy(players_t* players, dist_t* dist, uint16_t player_num, animal_t* animal);

void logic_animal_attack(players_t* player, dist_t* dist, uint16_t player_num, animal_t* animal);

#endif
