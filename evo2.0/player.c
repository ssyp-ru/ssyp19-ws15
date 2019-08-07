#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "animal.h"
#include "player.h"
#include "distributor.h"
#include "logic.h"
#include "hand.h"

player_t* create_new_player(){
	player_t* player = (player_t*)malloc(sizeof(player_t));
	player->hand = create_new_hand();
	player->head = NULL;
	player->animal_pt = NULL;
	player->trash = create_new_trash();
	player->animal_count = 0;
	player->weights = 0;
	return player;
}

void player_add_nothing_card(player_t* player, dist_t* dist, uint16_t player_num){
	add_card_to_hand(player->hand, dist->dict[21].f_name, dist->dict[21].s_name, &dist->dict[21].weights[player_num]);
}

void player_add_card(player_t* player, dist_t* dist, uint8_t num){
	player->animal_pt = dist->dict[20].f_name;
	dcard_t tmp = distributor_give_card(dist, num);
	add_card_to_hand(player->hand, tmp.f_name, tmp.s_name, tmp.weights);
}

void player_add_new_animal(player_t* player){
	animal_list_t* new_elem = (animal_list_t*)malloc(sizeof(animal_list_t));
	new_elem->animal = create_new_animal();
	new_elem->next = NULL;
	if (player->head == NULL)
		player->head = new_elem;
	else	{
		animal_list_t* tmp = player->head;
		while(tmp->next != NULL)
			tmp = tmp->next;
		tmp->next = new_elem;
	}
	player->animal_count++;
}

animal_t* player_return_animal(player_t* player, uint16_t index){
	if (index < player->animal_count){
		animal_list_t* tmp = player->head;
		for (uint16_t i = 0; i < index; i++)
			tmp = tmp->next;
		return tmp->animal;
	}
	return NULL;
}

void player_feed_animal(player_t* player, dist_t* dist){
	for (uint16_t i = 0; i < player->animal_count; i++){
		if (!player_animal_is_fed(player, i)){
			dist->food--;
			//player_animal_eat(player, i);
			animal_t* tmp = player_return_animal(player, i);
			for (uint16_t j = 0; j < tmp->used; j++){
				if (!strcmp(tmp->mods[j].name, "symbiosis") && !animal_is_fed(tmp->mods[j].s_animal)){
					//puts("sym");
					animal_eat(tmp->mods[j].s_animal);
					dist->food--;
					return;
				}
				if (!strcmp(tmp->mods[j].name, "communication")){
					//puts("com");
					animal_eat(tmp->mods[j].s_animal);
					dist->food--;
				}
				if (!strcmp(tmp->mods[j].name, "cooperation")){
					//puts("coo");
					animal_eat(tmp->mods[j].s_animal);
				}
			}
			//puts("other");
			player_animal_eat(player, i);
			break;
		}
		if (player_animal_has_empty_fat(player, i)){
			dist->food--;
			player_animal_add_fat(player, i);
			break;
		}
	}
}

animal_t* player_get_neighbour(player_t* player, uint16_t animal_num, uint8_t num){
	if (animal_num == 0)
		return player_return_animal(player, 1);
	if (animal_num == player->animal_count)
		return player_return_animal(player, animal_num - 1);
	if (num == 0)
		return player_return_animal(player, animal_num - 1);
	else
		return player_return_animal(player, animal_num + 1);
}

void player_delete_card_from_hand(player_t* player, uint16_t num){
	delete_card_from_hand(player->hand, num);
}

uint8_t player_animal_is_fed(player_t* player, uint16_t animal_num){
	animal_list_t* tmp = player->head;
	for (int i = 0; i < animal_num && tmp->next != NULL; i++)
		tmp = tmp->next;
	return animal_is_fed(tmp->animal);
}

uint8_t player_animal_has_empty_fat(player_t* player, uint16_t animal_num){
	animal_list_t* tmp = player->head;
	for (int i = 0; i < animal_num && tmp->next != NULL; i++)
		tmp = tmp->next;
	return animal_has_empty_fat(tmp->animal);
}

void player_animal_eat(player_t* player, uint16_t animal_num){
	animal_list_t* tmp = player->head;
	for (int i = 0; i < animal_num && tmp->next != NULL; i++)
		tmp = tmp->next;
	animal_eat(tmp->animal);
}

void player_animal_add_fat(player_t* player, uint16_t animal_num){
	animal_list_t* tmp = player->head;
	for (int i = 0; i < animal_num && tmp->next != NULL; i++)
		tmp = tmp->next;
	animal_add_fat(tmp->animal);
}

void player_animal_delete_food(player_t* player, uint16_t animal_num){
	animal_list_t* tmp = player->head;
	for (int i = 0; i < animal_num && tmp->next != NULL; i++)
		tmp = tmp->next;
	animal_delete_food(tmp->animal);
}

uint8_t player_all_animal_is_fed(player_t* player){
	uint8_t count = 0;
	animal_list_t* copy_head = player->head;
	while(copy_head != NULL){
		if (animal_is_fed(copy_head->animal) && !(animal_has_empty_fat(copy_head->animal)))
			count++;
		copy_head = copy_head->next;
	}
	if (count == player->animal_count)
		return 1;
	return 0;
}

uint16_t player_animals_weight(player_t* player){
	uint16_t count = 0;
	animal_list_t* copy_head = player->head;
	while(copy_head != NULL){
		count += animal_get_weight(copy_head->animal);
		copy_head = copy_head->next;
	}
	return count;
}

void player_animals_can_be_attack(find_t* find, player_t* player, animal_t* animal){
	animal_list_t* copy_head = player->head;
	while(copy_head != NULL){
		if (animal_can_be_attack(copy_head->animal, animal))
			add_animal_to_find(find, copy_head->animal);
		copy_head = copy_head->next;
	}
}

void player_find_hungry_animals(find_t* find, player_t* player){
	animal_list_t* copy_head = player->head;
	while(copy_head != NULL){
		if (!animal_is_fed(copy_head->animal) && copy_head->animal->eat > 0)
			add_animal_to_find(find, copy_head->animal);
		copy_head = copy_head->next;
	}
}

void player_has_animal_without_parasite(find_t* find, player_t* player){
	animal_list_t* copy_head = player->head;
	while(copy_head != NULL){
		if (!animal_has_parasite(copy_head->animal))
			add_animal_to_find(find, copy_head->animal);
		copy_head = copy_head->next;
	}
}

animal_list_t* player_re_list_animal(animal_list_t* head, animal_t* animal){
	animal_list_t* new_animal = (animal_list_t*)malloc(sizeof(animal_list_t));
	new_animal->animal = animal;
	new_animal->next = head;
	return new_animal;
}

void player_kill_hungry_animal(player_t* player){
	animal_list_t* copy_head = player->head;
	animal_list_t* re_list = NULL;
	while(copy_head != NULL){
		animal_list_t* tmp = copy_head->next;
		if (animal_is_fed(copy_head->animal) && !animal_has_black_tag(copy_head->animal))
			re_list = player_re_list_animal(re_list, copy_head->animal);
		else	{
			delete_animal(copy_head->animal);
			add_card_to_trash(player->trash, player->animal_pt);
			player->animal_count--;
		}
		free(copy_head);
		copy_head = tmp;
	}
	player->head = re_list;
}

void player_clear(player_t* player){
	while(player->head != NULL){
		animal_list_t* tmp = player->head->next;
		delete_animal(player->head->animal);
		free(player->head);
		player->head = tmp;
	}
	delete_hand(player->hand);
	delete_trash(player->trash);
	free(player);
}
