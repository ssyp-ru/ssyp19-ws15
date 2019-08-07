#ifndef _04_07_2019_PLAYER
#define _04_07_2019_PLAYER

#include "animal.h"
#include "hand.h"
#include "find.h"
#include "distributor.h"

typedef struct animal_list{
	animal_t* animal;
	struct animal_list* next;
}animal_list_t;

typedef struct player{
	animal_list_t* head;	
	hands_t* hand;
	trash_t* trash;
	uint8_t card_num;
	char* animal_pt;
	uint8_t animal_count;
	uint16_t weights;
}player_t;

player_t* create_new_player();

void player_add_nothing_card(player_t* player, dist_t* dist, uint16_t player_num);

void player_add_card(player_t* player, dist_t* dist, uint8_t num);

void player_add_new_animal(player_t* player);

animal_t* player_return_animal(player_t* player, uint16_t index);

void player_feed_animal(player_t* player, dist_t* dist);

animal_t* player_get_heighbour(player_t* player, uint16_t animal_num, uint8_t num);

uint8_t player_add_mod_to_animal(player_t* player, uint16_t animal_num, char* name, uint8_t weight);

void player_delete_mod_from_animal(player_t* player, uint16_t animal_num, uint8_t num_mod);

void player_delete_card_from_hand(player_t* player, uint16_t num);

uint8_t player_animal_is_fed(player_t* player, uint16_t animal_num);

uint16_t player_animals_weight(player_t* player);

uint8_t player_animal_has_empty_fat(player_t* player, uint16_t animal_num);

void player_animal_eat(player_t* player, uint16_t animal_num);

void player_animal_add_fat(player_t* player, uint16_t animal_num);

void player_animal_delete_food(player_t* player, uint16_t animal_num);

uint8_t player_all_animal_is_fed(player_t* player);

void player_animals_can_be_attack(find_t* find, player_t* player, animal_t* animal);

void player_find_hungry_animals(find_t* find, player_t* player);

void player_has_animal_without_parasite(find_t* find, player_t* player);

void player_print(player_t* player);

void player_print_hand(player_t* player);

void player_kill_hungry_animal(player_t* player);

void player_clear(player_t* player);

#endif
