#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "player.h"
#include "distributor.h"
#include "game.h"
#include "find.h"
#include "logic.h"

players_t* create_new_players(uint8_t num_players){
	players_t* new_players = (players_t*)malloc(sizeof(players_t));
	new_players->player = (player_t**)malloc(sizeof(player_t*) * num_players);
	for (int i = 0; i < num_players; i++){
		new_players->player[i] = create_new_player();
	}
	new_players->alloced = num_players;
	return new_players;
}

void game_get_nothing_card(dist_t* dist, players_t* players){
	for (uint16_t i = 0; i < players->alloced; i++){
		player_add_nothing_card(players->player[i], dist, i);
	}
}

void game_get_cards(dist_t* dist, players_t* players){
	for (int16_t i = 0; i < players->alloced; i++){
		if (players->player[i]->animal_count == 0)
			for (uint8_t j = 0; j < 6 && dist->deck != NULL; j++){
				player_add_card(players->player[i], dist, i);
			}
		else
			for (uint8_t j = 0; j < players->player[i]->animal_count + 1 && dist->deck != NULL; j++){
				player_add_card(players->player[i], dist, i);
			}
	}
}

uint8_t game_sum_voite(uint8_t* voites, uint16_t num){
	uint8_t sum = 0;
	for (uint16_t i = 0; i < num; i++)
		sum += voites[i];
	return sum;
}

void game_evolution(players_t* players, dist_t* dist){
	uint8_t* pass  = (uint8_t*)malloc(sizeof(uint8_t) * players->alloced);
	for (uint16_t i = 0; i < players->alloced; i++)
		pass[i] = 0;
	for (uint16_t i = 0; i < 500 && game_sum_voite(pass, players->alloced) != players->alloced; i = (i + 1) % players->alloced){
		if (players->player[i]->head == NULL){
			pass[i] = logic_create_new_animal(players->player[i]);
		}	else	{
			pass[i] = logic_use_card(players, dist, i);
		}

	}
	free(pass);
}

void game_re_food(dist_t* dist, players_t* players){
	distributor_re_food(dist, players->alloced);
}

void game_eat(dist_t* dist, players_t* players){
	uint16_t num_players = players->alloced;
	uint8_t* pass  = (uint8_t*)malloc(sizeof(uint8_t) * players->alloced);
	for (uint16_t i = 0; i < players->alloced; i++)
		pass[i] = 0;
	for (uint16_t i = 0; (dist->food > 0) && (game_sum_voite(pass, num_players) != num_players); i = (i + 1) % num_players){
		if (!player_all_animal_is_fed(players->player[i])){
			player_feed_animal(players->player[i], dist);
		}	else	{
			pass[i] = 1;
		}
	}
	free(pass);
}

void game_kill_animals(players_t* players){
	for (uint16_t i = 0; i < players->alloced; i++)
		player_kill_hungry_animal(players->player[i]);
}

void game_delete_food(players_t* players){
	for (uint16_t i = 0; i < players->alloced; i++)
		for (uint16_t j = 0; j < players->player[i]->animal_count; j++)
			player_animal_delete_food(players->player[i], j);
}

void delete_players(players_t* players){
	for (uint16_t i = 0; i < players->alloced; i++)
		player_clear(players->player[i]);
	free(players->player);
	free(players);
}
