#ifndef _04_07_2019_HAND
#define _04_07_2019_HAND

#include "card.h"
#include <stdint.h>

typedef struct hands{
	dcard_t* hand;
	uint16_t used;
	uint16_t alloced;
}hands_t;

typedef struct trash{
	tcard_t* trash;
	uint16_t used;
	uint16_t alloced;
}trash_t;

hands_t* create_new_hand();

trash_t* create_new_trash();

void resize_hand(hands_t* hand);

void resize_trash(trash_t* trash);

void add_card_to_hand(hands_t* hand, char* f_name, char* s_name, uint16_t* weights);

void add_card_to_trash(trash_t* trash, char* name);

void delete_card_from_hand(hands_t* hand, uint16_t num);

void delete_hand(hands_t* hand);

void delete_trash(trash_t* trash);

#endif
