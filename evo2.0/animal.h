#ifndef _04_07_2019_ANIMAL
#define _04_07_2019_ANIMAL

#include <stdint.h>

typedef struct card{
	char* name;
	struct animal* s_animal;
	uint8_t weight;
}card_t;

typedef struct animal{
	card_t* mods;
	uint8_t max_eat;
	uint8_t eat;
	uint8_t max_fat;
	uint8_t fat; 
	uint8_t black_tag;
	uint16_t used;
	uint16_t alloced;
}animal_t;

animal_t* create_new_animal();

void animal_mods_resize(animal_t* animal);

uint8_t animal_has_anti_perk(char* f_name, char* s_name);

uint8_t animal_has_perk(char* f_name);

uint8_t animal_can_be_attack(animal_t* f_animal, animal_t* s_animal);

uint8_t animal_is_carnivorous(animal_t* animal);

uint8_t animal_has_parasite(animal_t* animal);

uint8_t animal_is_poisonous(animal_t* animal);

uint8_t animal_is_fed(animal_t* animal);

uint8_t animal_has_black_tag(animal_t* animal);

uint8_t animal_has_empty_fat(animal_t* animal);

void animal_add_fat(animal_t* animal);

void animal_eat(animal_t* animal);

void animal_delete_food(animal_t* animal);

void animal_add_mods(animal_t* animal, char* name, uint8_t weight);

uint8_t animal_can_be_symbiont(animal_t* f_animal, animal_t* s_animal);

void animal_add_symbiosis(animal_t* f_animal, animal_t* s_animal, char* name, uint8_t weight);

uint16_t animal_get_weight(animal_t* animal);

void animal_add_double_mods(animal_t* f_animal, animal_t* s_animal, char* name, uint8_t weight);

//void animal_print_mods(animal_t* animal);

char* animal_delete_mod(animal_t* animal, uint16_t num);

void animal_kill(animal_t* animal);

void delete_animal(animal_t* animal);

#endif
