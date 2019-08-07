#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "find.h"
#include "card.h"
#include "player.h"
#include "animal.h"
#include "logic.h"

typedef struct win{
		uint16_t pos;
		uint16_t weight;
}win_t;

int logic_min_to_max(const void* val1, const void* val2){
	int f_sum = 0;
	int s_sum = 0;
	if (((dcard_t*)val1)->weights != NULL && ((dcard_t*)val2)->weights != NULL){
		uint16_t f_num = ((dcard_t*)val1)->weights[0];
		uint16_t s_num = ((dcard_t*)val2)->weights[0];
		uint8_t f_f = (f_num & 0xFF00) >> 8;
		uint8_t f_s = f_num & 0x00FF;
		uint8_t s_f = (s_num & 0xFF00) >> 8;
		uint8_t s_s = s_num & 0x00FF;	
		f_sum = (f_f > f_s) ? f_f : f_s;
		s_sum = (s_f > s_s) ? s_f : s_s;
	}
	return (f_sum - s_sum);
}

int logic_max_to_min(const void* val1, const void* val2){
	int f_sum = 0;
	int s_sum = 0;
	if (((dcard_t*)val1)->weights != NULL && ((dcard_t*)val2)->weights != NULL){
		uint16_t f_num = ((dcard_t*)val1)->weights[0];
		uint16_t s_num = ((dcard_t*)val2)->weights[0];
		uint8_t f_f = (f_num & 0xFF00) >> 8;
		uint8_t f_s = f_num & 0x00FF;
		uint8_t s_f = (s_num & 0xFF00) >> 8;
		uint8_t s_s = s_num & 0x00FF;	
		f_sum = (f_f > f_s) ? f_f : f_s;
		s_sum = (s_f > s_s) ? s_f : s_s;
	}
	return (s_sum - f_sum);
}

void magic_print(player_t* player){
	animal_list_t* copy_head = player->head;
	uint16_t num = 0;
	while(copy_head != NULL){
		if(copy_head->animal == NULL)
			printf("%d:+", num);
		else
			printf("%d:-", num);
		num++;
		copy_head = copy_head->next;
	}
}

uint8_t logic_create_new_animal(player_t* player){
	if (player->hand->used > 1){
		qsort(player->hand->hand, player->hand->used, sizeof(dcard_t), logic_min_to_max);
		player_add_new_animal(player);
		if (strcmp(player->hand->hand[0].f_name, "nothing")){
			player_delete_card_from_hand(player, 1);
			return 0;
		}	else	{
			player_delete_card_from_hand(player, 0);
			return 0;
		}
		/*for (uint16_t i = 0; i < player->hand->used; i++){
			if (strcmp(player->hand->hand[i].f_name, "nothing")){
				player_delete_card_from_hand(player, i);
				//player_add_new_animal(player);
				//magic_print(player);
				return 0;
			}
		}*/
	}
	return 1;
}

animal_t* logic_get_neighbour(player_t* player, uint16_t animal_num, uint8_t num){
	if (animal_num == 0)
		return player_return_animal(player, 1);
	if (animal_num == player->animal_count - 1)
		return player_return_animal(player, animal_num - 1);
	if (num == 0)
		return player_return_animal(player, animal_num - 1);
	else
		return player_return_animal(player, animal_num + 1);
}

uint8_t logic_use_double_mods(player_t* player){
	uint8_t* array = (uint8_t*)calloc(sizeof(uint8_t) * player->animal_count, sizeof(uint8_t) * player->animal_count);
	while(game_sum_voite(array, player->animal_count) < player->animal_count){
		animal_list_t* copy_head = player->head;
		animal_t* max_animal = NULL;
		int16_t max_weight = -1;
		uint16_t counter = 0;
		while(copy_head != NULL){
			if (animal_get_weight(copy_head->animal) > max_weight && array[counter] == 0){
				max_weight = animal_get_weight(copy_head->animal);
				max_animal = copy_head->animal;
			}
			counter++;
			copy_head = copy_head->next;
		}
		animal_t* copy_animal = NULL;
		animal_t* neighbour_used = NULL;
		uint8_t count = 0;
		uint8_t flag = 0;
		for (uint16_t i = 0; i < max_animal->used; i++){
			if (!strcmp(max_animal->mods[i].name, player->hand->hand[0].f_name)){
				count++;
				neighbour_used = max_animal->mods[i].s_animal;
				if (count == 2){
					array[counter] = 0;
					flag = 1;
					break;
				}
			}
		}
		if (flag == 1)
			continue;
		for (uint8_t i = 0; i < 2 && copy_animal == NULL; i++){
			copy_animal = logic_get_neighbour(player, counter, i);
			if (copy_animal == neighbour_used)
				copy_animal = NULL;
			}
			if (copy_animal == NULL)
				continue;
			else{
				animal_add_double_mods(max_animal, copy_animal, player->hand->hand[0].f_name, get_first_state(*player->hand->hand[0].weights));
				free(array);
				return 0;
			}
	}
	free(array);
	return 1;
}

uint8_t logic_use_symbiosis(player_t* player){
	uint8_t* array = (uint8_t*)calloc(sizeof(uint8_t) * player->animal_count, sizeof(uint8_t) * player->animal_count);
	while(game_sum_voite(array, player->animal_count) < player->animal_count){
		animal_list_t* copy_head = player->head;
		animal_t* max_animal = NULL;
		int16_t max_weight = -1;
		uint16_t counter = 0;
		while(copy_head != NULL){
			if (animal_get_weight(copy_head->animal) > max_weight && array[counter] != 1){
				max_weight = animal_get_weight(copy_head->animal);
				max_animal = copy_head->animal;
			}
			counter++;
			copy_head = copy_head->next;
		}
		animal_t* copy_animal = NULL;
		animal_t* neighbour_used = NULL;
		uint8_t count = 0;
		uint8_t flag = 0;
		for (uint16_t i = 0; i < max_animal->used; i++){
			if (!strcmp(max_animal->mods[i].name, player->hand->hand[0].f_name)){
				count++;
				neighbour_used = max_animal->mods[i].s_animal;
				if (count == 2){
					array[counter] = 0;
					flag = 1;
					break;
				}
			}
		}
		if (flag == 1)
			continue;
		for (uint8_t i = 0; i < 2 && copy_animal == NULL; i++){
			copy_animal = logic_get_neighbour(player, counter, i);
			if (copy_animal == neighbour_used)
				copy_animal = NULL;
			}
			if (copy_animal == NULL)
				continue;
			else{
				animal_add_symbiosis(max_animal, copy_animal, player->hand->hand[0].f_name, get_first_state(*player->hand->hand[0].weights));
				free(array);
				return 0;
			}
	}
	free(array);
	return 1;
}

uint8_t logic_protect_animal(players_t* players, animal_t* animal, uint16_t player_num){
	find_t* find_car = create_new_find();
	player_t* player = players->player[player_num];
	for(uint16_t i = 0; i < players->alloced; i++){
		if (i != player_num){
			animal_list_t* copy_head = players->player[i]->head;
			while(copy_head != NULL){
				if (animal_is_carnivorous(copy_head->animal) && animal_can_be_attack(animal, copy_head->animal))
					add_animal_to_find(find_car, copy_head->animal);
				copy_head = copy_head->next;
			}
		}
	}
	uint8_t* array = (uint8_t*)malloc(sizeof(uint8_t*) * find_car->used);
	for (uint16_t i = 0; i < find_car->used; i++)
		array[i] = 0;
	for (uint16_t i = 0; i < find_car->used; i++){
		for (uint16_t j = 0; j < find_car->animals[i]->used; j++){
			if (!animal_has_anti_perk(player->hand->hand[0].f_name, find_car->animals[i]->mods[j].name))
				array[i] = 1;
			else
				array[i] = 0;
		}
	}
	if (game_sum_voite(array, find_car->used) > 0){
		animal_add_mods(animal, player->hand->hand[0].f_name, get_first_state(*player->hand->hand[0].weights));
		delete_find(find_car);
		return 0;
	}
	delete_find(find_car);
	return 1;
}

uint8_t logic_use_hibernation_ability(player_t* player){
	uint16_t max_eat = 0;
	animal_list_t* copy_head = player->head;
	for(; copy_head != NULL; copy_head = copy_head->next)
		if (max_eat < copy_head->animal->max_eat)
			max_eat = copy_head->animal->max_eat;
	animal_t* copy_animal = NULL;
	uint16_t max_weight = 0;
	copy_head = player->head;
	for (; copy_head != NULL; copy_head = copy_head->next)
		if (max_eat == copy_head->animal->max_eat && animal_get_weight(copy_head->animal) > max_weight){
			max_weight = animal_get_weight(copy_head->animal);
			copy_animal = copy_head->animal;
		}
	if (copy_animal != NULL){
		animal_add_mods(copy_animal, player->hand->hand[0].f_name, get_first_state(*player->hand->hand[0].weights));
		return 0;
	}
	return 1;
}

void logic_use_other_card(player_t* player, uint8_t num){
	uint16_t animal = rand() % player->animal_count;
	animal_list_t* copy_head = player->head;
	dcard_t* card = &player->hand->hand[0];
	for (uint16_t i = 0; i < animal; i++)
		copy_head = copy_head->next;
	if(num == 0)
		animal_add_mods(copy_head->animal, card->f_name, get_first_state(*card->weights));
	else
		animal_add_mods(copy_head->animal, card->f_name, get_second_state(*card->weights));
}

uint8_t logic_use_card(players_t* players, dist_t* dist, uint16_t player_num){
	if (players->player[player_num]->hand->used > 0){
		uint8_t rand_num = rand() % 3;
		if(rand_num == 0)
			return logic_create_new_animal(players->player[player_num]);
		else
			if (logic_add_mods(players, dist, player_num) == 0){
				delete_card_from_hand(players->player[player_num]->hand, 0);
				return 0;
			}
	}
	return 1;
}

uint8_t logic_add_mods(players_t* players, dist_t* dist, uint16_t player_num){
	player_t* player = players->player[player_num];
	//uint8_t* array = (uint8_t*)calloc(sizeof(uint8_t) * player->animal_count, sizeof(uint8_t) * player->animal_count);
	if (player->hand->used > 0){
		qsort(players->player[player_num]->hand->hand, player->hand->used, sizeof(dcard_t), logic_max_to_min);
		if (get_first_state(*player->hand->hand[0].weights) > get_second_state(*player->hand->hand[0].weights)){
			if (!strcmp(player->hand->hand[0].f_name, "parasite")){
				logic_add_parasite_to_animal(players, dist, player_num, get_first_state(*player->hand->hand[0].weights));
				return 0;
			}
			if (!strcmp(player->hand->hand[0].f_name, "nothing")){
				return 1;
			}
			if (!strcmp(player->hand->hand[0].f_name, "communication")){
				return logic_use_double_mods(player);
			}
			if (!strcmp(player->hand->hand[0].f_name, "cooperation")){
				return logic_use_double_mods(player);
			}
			if (!strcmp(player->hand->hand[0].f_name, "symbiosis")){
				return logic_use_double_mods(player);
			}
			if (animal_has_perk(player->hand->hand[0].f_name)){
				player_t* copy_player = players->player[player_num];
				animal_list_t* copy_head = players->player[player_num]->head;
				uint8_t flag = 1;
				for (uint16_t i = 0; i < copy_player->animal_count && flag; i++){
					flag = logic_protect_animal(players, copy_head->animal, player_num);
					copy_head = copy_head->next;
				}
				return 0;
			}
			if (!strcmp(player->hand->hand[0].f_name, "hibernation ability")){
				return logic_use_hibernation_ability(player);
			}
			logic_use_other_card(player, 0);
		}	else
			logic_use_other_card(player, 1);
	}
	//free(array);
	return 0;
}

uint8_t get_first_state(uint16_t num){
	return (num & 0xFF00) >> 8;
}

uint8_t get_second_state(uint16_t num){
	return num & 0x00FF;
}

void logic_use_piracy(players_t* players, dist_t* dist, uint16_t player_num, animal_t* animal){
	find_t* find_hungry = create_new_find();
	for (uint16_t i = 0; i < players->alloced; i++){
		if (i != player_num)
			player_find_hungry_animals(find_hungry, players->player[i]);
	}
	if (find_hungry->used > 0){
		animal_t* tmp = NULL;
		uint16_t max_sum = 0;
		uint16_t sum = 0;
		for (uint16_t i = 0; i < find_hungry->used; i++){
			for (uint16_t j = 0; j < find_hungry->animals[i]->used; j++){
				for (uint8_t k = 0; k < 20; k++){
					if (!strcmp(find_hungry->animals[i]->mods[j].name, dist->dict[k].f_name)){
						sum += dist->dict[k].weights[player_num];
						break;
					}
					if (dist->dict[k].s_name != NULL && !strcmp(find_hungry->animals[i]->mods[j].name, dist->dict[k].s_name)){
						sum += dist->dict[k].weights[player_num];
						break;
					}
				}
			}
			if (sum > max_sum){
				tmp = find_hungry->animals[i];
				max_sum = sum;
			}
		}
		if (tmp != NULL){
			tmp->eat--;
			animal->eat++;
		}
	}
	delete_find(find_hungry);
}

uint8_t logic_add_parasite_to_animal(players_t* players, dist_t* dist, uint16_t player_num, uint8_t weight){
	find_t* find_parasite = create_new_find();
	for (uint16_t i = 0; i < players->alloced; i++){
		if (i != player_num)
			player_has_animal_without_parasite(find_parasite, players->player[i]);
	}
	if (find_parasite->used > 0){
		animal_t* tmp = NULL;
		uint16_t max_sum = 0;
		uint16_t sum = 0;
		for (uint16_t i = 0; i < find_parasite->used; i++){
			for (uint16_t j = 0; j < find_parasite->animals[i]->used; j++){
				for (uint8_t k = 0; k < 20; k++){
					if (!strcmp(find_parasite->animals[i]->mods[j].name, dist->dict[k].f_name)){
						sum += dist->dict[k].weights[player_num];
						break;
					}
					if (dist->dict[k].s_name != NULL && !strcmp(find_parasite->animals[i]->mods[j].name, dist->dict[k].s_name)){
						sum += dist->dict[k].weights[player_num];
						break;
					}
				}
			}
			if (sum > max_sum){
				tmp = find_parasite->animals[i];
				max_sum = sum;
			}
		}
		delete_find(find_parasite);
		if (tmp != NULL){
			animal_add_mods(tmp, "parasite", weight);
		}	else
			return 1;
	}
	return 0;
}

void logic_animal_attack(players_t* player, dist_t* dist, uint16_t player_num, animal_t* animal){
	find_t* find_animal = create_new_find();
	for (uint16_t i = 0; i < player->alloced; i++){
		if (i != player_num)
			player_animals_can_be_attack(find_animal, player->player[i], animal);
	}
	if (find_animal->used > 0){
		animal_t* tmp = NULL;
		uint16_t max_sum = 0;
		uint16_t sum = 0;
		for (uint16_t i = 0; i < find_animal->used; i++){
			for (uint16_t j = 0; j < find_animal->animals[i]->used; j++){
				for (uint8_t k = 0; k < 20; k++){
					if (!strcmp(find_animal->animals[i]->mods[j].name, dist->dict[k].f_name)){
						sum += dist->dict[k].weights[player_num];
						break;
					}
					if (dist->dict[k].s_name != NULL && !strcmp(find_animal->animals[i]->mods[j].name, dist->dict[k].s_name)){
						sum += dist->dict[k].weights[player_num];
						break;
					}
				}
			}
			if (sum > max_sum){
				tmp = find_animal->animals[i];
				max_sum = sum;
			}
		}	
		if (tmp != NULL){
			animal_kill(tmp);
			if (animal_has_black_tag(animal) && animal_is_poisonous(animal))
				animal_kill(animal);
			delete_find(find_animal);
		}
	}
}

int logic_winer_sort(const void* val1, const void* val2){
	uint16_t f_weight = ((win_t*)val1)->weight;
	uint16_t s_weight = ((win_t*)val2)->weight;
	return f_weight - s_weight;
}

void logic_who_win(players_t* players, dist_t* dist){
	FILE* f_output = fopen("first.log", "w");
	FILE* s_output = fopen("second.log", "w");	
	for (uint16_t i = 0; i < players->alloced; i++){
		animal_list_t* copy_head = players->player[i]->head;
		for (; copy_head != NULL; copy_head = copy_head->next){
			for (uint16_t j = 0; j < copy_head->animal->used; j++){
				players->player[i]->weights++;
				if (!strcmp(copy_head->animal->mods[i].name, "high body weight"))
					players->player[i]->weights++;
				if (!strcmp(copy_head->animal->mods[i].name, "carnivorous"))
					players->player[i]->weights++;
				if (!strcmp(copy_head->animal->mods[i].name, "parasite"))
					players->player[i]->weights += 2;
			}
			players->player[i]->weights++;
		}
	}
	win_t* copy_players = (win_t*)calloc(sizeof(win_t) * players->alloced, sizeof(win_t) * players->alloced);
	for (uint16_t i = 0; i < players->alloced; i++){
		copy_players->pos = i;
		copy_players->weight = players->player[i]->weights;
	}
	//qsort(players->player, players->alloced, sizeof(win_t), logic_winer_sort);
	for (uint16_t i = 0; i < 20; i++){
		fprintf(f_output, "%hu\n", dist->dict[i].weights[copy_players[0].pos]);
		fprintf(s_output, "%hu\n", dist->dict[i].weights[copy_players[1].pos]);
	}
	fprintf(f_output, "%hu\n", dist->dict[21].weights[copy_players[0].pos]);
	fprintf(s_output, "%hu\n", dist->dict[21].weights[copy_players[1].pos]);
	fclose(f_output);
	fclose(s_output);
}
