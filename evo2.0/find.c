#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "animal.h"
#include "hand.h"
#include "find.h"

find_t* create_new_find(){
	find_t* new_find = (find_t*)malloc(sizeof(find_t));
	new_find->animals = (animal_t**)malloc(sizeof(animal_t*));
	new_find->used = 0;
	new_find->alloced = 1;
	return new_find;
}

void resize_find(find_t* find){
	animal_t** tmp = (animal_t**)malloc(sizeof(animal_t*) * find->alloced * 2);
	for (uint16_t i = 0; i < find->used; i++)
		tmp[i] = find->animals[i];
	free(find->animals);
	find->animals = tmp;
	find->alloced *= 2;
}

void add_animal_to_find(find_t* find, animal_t* animal){
	if (find->alloced - find->used == 0)
		resize_find(find);
	find->animals[find->used] = animal;
	find->used++;
}

void delete_find(find_t* find){
	free(find->animals);
	free(find);
}
