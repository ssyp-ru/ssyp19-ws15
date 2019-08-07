#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <time.h>
#include "distributor.h"

void distributor_add_card_to_deck(dist_t* dist, char* f_name, char* s_name, uint16_t* weights, uint8_t index){
	deck_t* new_card = (deck_t*)malloc(sizeof(deck_t));
	new_card->card.f_name = f_name;
	new_card->card.s_name = s_name;
	new_card->card.weights = weights;
	new_card->next = NULL;
	if (dist->deck == NULL){
		dist->deck = new_card;
	}	else	{
		if (index == 0){
			new_card->next = dist->deck;
			dist->deck = new_card;
		}	else	{
			deck_t* tmp = dist->deck;
			for (uint16_t i = 0; i < index && tmp->next != NULL; i++)
				tmp = tmp->next;
			deck_t* copy_next = tmp->next;
			tmp->next = new_card;
			new_card->next = copy_next;
		}
	}
	dist->len++;
}

void add_card_to_dict(dist_t* dist, char* f_name, char* s_name){
	dist->dict[dist->used].f_name = (char*)malloc(sizeof(char) * (strlen(f_name) + 1));
	strcpy(dist->dict[dist->used].f_name, f_name);
	dist->dict[dist->used].weights = (uint16_t*)malloc(sizeof(uint16_t) * dist->players_num);
	if (s_name != NULL){
		dist->dict[dist->used].s_name = (char*)malloc(sizeof(char) * (strlen(s_name) + 1));
		strcpy(dist->dict[dist->used].s_name, s_name);
	}
	dist->used++;
}

dist_t* create_new_distributor(uint8_t players){
	dist_t* dist = (dist_t*)malloc(sizeof(dist_t));
	dist->dict = (dcard_t*)malloc(sizeof(dcard_t) * 22);
	dist->deck = NULL;
	dist->food = 0;
	dist->used = 0;
	dist->len = 0;
	dist->players_num = players;
	add_card_to_dict(dist, "camouflage", "fat tissue");
	add_card_to_dict(dist, "burrowing", "fat tissue");
	add_card_to_dict(dist, "sharp vision", "fat tissue");
	add_card_to_dict(dist, "symbiosis", NULL);
	add_card_to_dict(dist, "piracy", NULL);
	add_card_to_dict(dist, "grazing", "fat tissue");
	add_card_to_dict(dist, "tail loss", NULL);
	add_card_to_dict(dist, "hibernation ability", "carnivorous");
	add_card_to_dict(dist, "poisonous", "carnivorous");
	add_card_to_dict(dist, "communication", "carnivorous");
	add_card_to_dict(dist, "scavenger", NULL);
	add_card_to_dict(dist, "running", NULL);
	add_card_to_dict(dist, "mimicry", NULL);
	add_card_to_dict(dist, "swimming", NULL);
	add_card_to_dict(dist, "parasite", "carnivorous");
	add_card_to_dict(dist, "parasite", "fat tissue");
	add_card_to_dict(dist, "cooperation", "carnivorous");
	add_card_to_dict(dist, "cooperation", "fat tissue");
	add_card_to_dict(dist, "high body weight", "carnivorous");
	add_card_to_dict(dist, "high body weight", "fat tissue");
	add_card_to_dict(dist, "animal", NULL);
	add_card_to_dict(dist, "nothing", NULL);
	for (uint8_t i = 0; i < 20; i++)
		for (uint8_t j = 0; j < 4; j++)
			distributor_add_card_to_deck(dist, dist->dict[i].f_name, dist->dict[i].s_name, dist->dict[i].weights, rand() % 84);
	for (uint8_t j = 0; j < 4; j++)
		distributor_add_card_to_deck(dist, dist->dict[12].f_name, dist->dict[12].s_name, dist->dict[12].weights, rand() % 84);
	return dist;
}

void distributor_load_weights(dist_t* dist){
	FILE* f_input = fopen("first.log", "r");
	FILE* s_input = fopen("second.log", "r");
	for (uint8_t i = 0; i < 20; i++){
		fscanf(f_input, "%hu", &dist->dict[i].weights[0]);
		fscanf(s_input, "%hu", &dist->dict[i].weights[1]);
		uint8_t rand_num = rand() % 2;
		if (rand_num)
			dist->dict[i].weights[2] = dist->dict[i].weights[0];
		else
			dist->dict[i].weights[2] = dist->dict[i].weights[1];
		rand_num = rand() % 10;
		if (rand_num == 0)
			dist->dict[i].weights[3] = rand() % 65536;
		else	{
			rand_num = rand() % 2;
			if (!rand_num)
				dist->dict[i].weights[3] = dist->dict[i].weights[0];
			else
				dist->dict[i].weights[3] = dist->dict[i].weights[1];
		}
	}
	fscanf(f_input, "%hu", &dist->dict[21].weights[0]);
	fscanf(s_input, "%hu", &dist->dict[21].weights[1]);
	uint8_t rand_num = rand() % 2;
	if (rand_num)
		dist->dict[21].weights[2] = dist->dict[21].weights[0];
	else
		dist->dict[21].weights[2] = dist->dict[21].weights[1];
	rand_num = rand() % 10;
	if (rand_num == 0)
		dist->dict[21].weights[3] = rand() % 65536;
	else	{
		rand_num = rand() % 2;
		if (!rand_num)
			dist->dict[21].weights[3] = dist->dict[21].weights[0];
		else
			dist->dict[21].weights[3] = dist->dict[21].weights[1];
	}
	fclose(f_input);
	fclose(s_input);
}

dcard_t distributor_give_card(dist_t* dist, uint8_t num){
	dcard_t tmp;
	deck_t* copy_deck = dist->deck->next;
	tmp.f_name = dist->deck->card.f_name;
	tmp.s_name = dist->deck->card.s_name;
	tmp.weights = &dist->deck->card.weights[num];
	free(dist->deck);
	dist->len--;
	dist->deck = copy_deck;
	return tmp;
}

void distributor_re_food(dist_t* dist, uint8_t player_num){
	switch(player_num){
		case 2:
			dist->food = (rand() % 6 + 1) + 2;
		break;
		case 3:
			dist->food = (rand() % 6 + 1) + (rand() % 6 + 1);
		break;
		case 4:
			dist->food = (rand() % 6 + 1) + (rand() % 6 + 1) + 2;
		break;
	}
}

void delete_distributor(dist_t* dist){
	for (uint8_t i = 0; i < 21; i++){
		free(dist->dict[i].f_name);
		free(dist->dict[i].s_name);
		free(dist->dict[i].weights);
	}
	free(dist->dict);
	free(dist);
}
