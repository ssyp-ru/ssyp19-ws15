#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "animal.h"

animal_t* create_new_animal(){
	animal_t* animal = (animal_t*)malloc(sizeof(animal_t));
	animal->mods = (card_t*)malloc(sizeof(card_t));
	animal->used = 0;
	animal->alloced = 1;
	animal->max_eat = 1;
	animal->eat = 0;
	animal->max_fat = 0;
	animal->fat = 0;
	return animal;
}

void animal_mods_resize(animal_t* animal){
	card_t* tmp = (card_t*)malloc(sizeof(card_t) * animal->alloced * 2);
	for (int i = 0; i < animal->used; i++){
		tmp[i].name = animal->mods[i].name;
		tmp[i].s_animal = animal->mods[i].s_animal;
		tmp[i].weight = animal->mods[i].weight;
	}
	free(animal->mods);
	animal->mods = tmp;
	animal->alloced *= 2;
}

uint8_t animal_has_anti_perk(char* f_name, char* s_name){
	if (!strcmp(f_name, "high body weight") && !strcmp(s_name, "high body weight"))
		return 1;
	if (!strcmp(f_name, "camouflage") && !strcmp(s_name, "sharp vision"))
		return 1;
	if (!strcmp(f_name, "swimming") && !strcmp(s_name, "swimming"))
		return 1;
	if (!strcmp(f_name, "running"))
		return 1;
	if (!strcmp(f_name, "tail loss"))
		return 1;
	if (!strcmp(f_name, "poisonous"))
		return 1;
	if (!strcmp(f_name, "mimicry"))
		return 1;
	//if (!strcmp(f_name, "cam") && !strcmp(s_name, "sharp vision"))
	//	return 1;
	return 0;
}

uint8_t animal_has_perk(char* f_name){
	if (!strcmp(f_name, "high body weight"))
		return 1;
	if (!strcmp(f_name, "camouflage"))
		return 1;
	if (!strcmp(f_name, "swimming"))
		return 1;
	if (!strcmp(f_name, "running"))
		return 1;
	if (!strcmp(f_name, "camouflage"))
		return 1;
	if (!strcmp(f_name, "burrowing"))
		return 1;
	if (!strcmp(f_name, "tail loss"))
		return 1;
	if (!strcmp(f_name, "poisonous"))
		return 1;
	if (!strcmp(f_name, "mimicry"))
		return 1;
	return 0;
}

uint8_t animal_can_be_attack(animal_t* f_animal, animal_t* s_animal){
	uint8_t flag = 1;
	for (uint16_t i = 0; i < f_animal->used && flag != 0; i++){
		for (uint16_t j = 0; j < s_animal->used; j++){
			if (animal_has_perk(f_animal->mods[i].name)){
				if (!animal_has_anti_perk(f_animal->mods[i].name, s_animal->mods[j].name)){
					flag = 0;
					break;
				}
				if (!strcmp(f_animal->mods[i].name, "burrowing") && animal_is_fed(f_animal)){
					flag = 0;
					break;
				}
			}
		}
	}
	return flag;
}

uint8_t animal_is_carnivorous(animal_t* animal){
	uint8_t flag = 0;
	for (uint16_t i = 0; i < animal->used; i++){
		if (!strcmp(animal->mods[i].name, "carnivorous"))
			flag = 1;
	}
	return flag;
}

uint8_t animal_has_parasite(animal_t* animal){
	uint8_t flag = 0;
	for (uint16_t i = 0; i < animal->used; i++){
		if (!strcmp(animal->mods[i].name, "parasite"))
			flag = 1;
	}
	return flag;
}

uint8_t animal_is_poisonous(animal_t* animal){
	uint8_t flag = 0;
	for (uint16_t i = 0; i < animal->used; i++){
		if (!strcmp(animal->mods[i].name, "poisonous"))
			flag = 1;
	}
	return flag;
}

uint8_t animal_is_fed(animal_t* animal){
	if (animal == NULL || animal->eat == animal->max_eat || animal->black_tag)
		return 1;
	return 0;
}

uint8_t animal_has_black_tag(animal_t* animal){
	return animal->black_tag;
}

uint8_t animal_has_empty_fat(animal_t* animal){
	if (animal->fat < animal->max_fat)
		return 1;
	return 0;
}

void animal_add_fat(animal_t* animal){
	animal->fat++;
}

void animal_eat(animal_t* animal){
	if (animal != NULL)
		animal->eat++;
}

void animal_delete_food(animal_t* animal){
	animal->eat = 0;
}

void animal_add_mods(animal_t* animal, char* name, uint8_t weight){
	if (animal->alloced - animal->used == 0)
		animal_mods_resize(animal);
	animal->mods[animal->used].name = name;
	animal->mods[animal->used].s_animal = NULL;
	animal->mods[animal->used].weight = weight;
	animal->used++;
	if (!strcmp(name, "fat tissue"))
		animal->max_fat++;
	if (!strcmp(name, "parasite"))
		animal->max_eat += 2;
	if (!strcmp(name, "high body weight"))
		animal->max_eat++;
	if (!strcmp(name, "carnivorous"))
		animal->max_eat++;
}

uint8_t animal_can_be_symbiont(animal_t* f_animal, animal_t* s_animal){
	uint8_t flag = 1;
	for (uint16_t i = 0; i < s_animal->used; i++)
		if (!strcmp(s_animal->mods[i].name, "symbiosis") && s_animal->mods[i].s_animal == f_animal)
			flag = 0;
	return flag;
}

int animal_min_to_max(const void* val1, const void* val2){
	uint8_t f_weight = ((card_t*)val1)->weight;
	uint8_t s_weight = ((card_t*)val2)->weight;
	return f_weight - s_weight;
}

void animal_use_tail_loss(animal_t* animal){
	qsort(animal->mods, animal->used, sizeof(card_t), animal_min_to_max);
	animal_delete_mod(animal, 0);
}

void animal_add_symbiosis(animal_t* f_animal, animal_t* s_animal, char* name, uint8_t weight){
	if(f_animal->alloced - f_animal->used == 0)
		animal_mods_resize(f_animal);
	f_animal->mods[f_animal->used].name = name;
	f_animal->mods[f_animal->used].s_animal = s_animal;
	f_animal->mods[f_animal->used].weight = weight;
	f_animal->used++;
}

uint16_t animal_get_weight(animal_t* animal){
	uint16_t sum = 0;
	for (uint16_t i = 0; i < animal->used; i++)
		sum += animal->mods[i].weight;
	return sum;
}

void animal_add_double_mods(animal_t* f_animal, animal_t* s_animal, char* name, uint8_t weight){
	if(f_animal->alloced - f_animal->used == 0)
		animal_mods_resize(f_animal);
	if(s_animal->alloced - s_animal->used == 0)
		animal_mods_resize(s_animal);
	f_animal->mods[f_animal->used].name = name;
	f_animal->mods[f_animal->used].s_animal = s_animal;
	f_animal->mods[f_animal->used].weight = weight;
	f_animal->used++;
	s_animal->mods[s_animal->used].name = name;
	s_animal->mods[s_animal->used].s_animal = f_animal;
	s_animal->mods[s_animal->used].weight = weight;
	s_animal->used++;
}

char* animal_delete_mod(animal_t* animal, uint16_t num){
	char* tmp = NULL;
	if (num <= animal->used){
		tmp = animal->mods[num].name;
		for (int i = 0; i < animal->used - num - 1; i++){
			animal->mods[num + i].name = animal->mods[num + i + 1].name;
		}
		animal->used--;
	}
	return tmp;
}

void animal_kill(animal_t* animal){
	uint8_t flag = 1;
	qsort(animal->mods, animal->used, sizeof(card_t), animal_min_to_max);
	for (uint16_t i = 0; i < animal->used; i++){
		if (!strcmp(animal->mods[i].name, "running")){
			uint8_t rand_int = rand() % 6;
			if (rand_int > 2)
				flag = 0;
		}
		if (!strcmp(animal->mods[i].name, "tail loss")){
			animal_use_tail_loss(animal);
			flag = 0;
		}
	}
	if (flag)
		animal->black_tag = 1;
}

void delete_animal(animal_t* animal){
	for (uint16_t i = 0; i < animal->used; i++)
		if (!strcmp(animal->mods[i].name, "communication") || !strcmp(animal->mods[i].name, "cooperation")){
			animal_t* copy_animal = animal->mods[i].s_animal;
			for (uint16_t j = 0;copy_animal != NULL && j < copy_animal->used; j++){
				if (!strcmp(copy_animal->mods[j].name, animal->mods[i].name) && copy_animal->mods[j].s_animal == animal){
					animal_delete_mod(copy_animal, j);
					break;
				}
			}
		}
	free(animal->mods);
	free(animal);
}
