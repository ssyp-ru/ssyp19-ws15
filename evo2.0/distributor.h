#ifndef _04_07_DISTRIBUTION
#define _04_07_DISTRIBUTION

#include <stdint.h>
#include "card.h"

/*typedef struct dist_card{
	char* f_name;
	char* s_name;
	//uint8_t num;
}dcard_t;*/

typedef struct deck_list{
	dcard_t card;
	struct deck_list* next;
}deck_t;

typedef struct distributor{
	//dcard_t* deck;
	dcard_t* dict;
	uint8_t used;
	deck_t* deck;
	uint16_t len;
	uint8_t food;
	uint8_t players_num;
}dist_t;

void distributor_add_card_to_deck(dist_t* deck, char* f_name, char* s_name, uint16_t* weights, uint8_t index);

void add_card_to_dict(dist_t* dist, char* f_name, char* s_name);

dist_t* create_new_distributor(uint8_t players);

void distributor_load_weights(dist_t* dist);

dcard_t distributor_give_card(dist_t* dist, uint8_t num);

void distributor_re_food(dist_t* dist, uint8_t player_num);

void delete_distributor(dist_t* dist);

#endif
