#ifndef _09_07_2019_FIND
#define _09_07_2019_FIND

#include "animal.h"

typedef struct find_animals{
	animal_t** animals;
	uint16_t used;
	uint16_t alloced;
}find_t;

find_t* create_new_find();

void resize_find(find_t* find);

void add_animal_to_find(find_t* find, animal_t* animal);

//void has_animal_without_parasite(player_t* player, find_t* find);

void delete_find(find_t* find);

#endif
