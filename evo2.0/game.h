#ifndef _06_07_2019_GAME
#define _06_07_2019_GAME

#include "player.h"
#include "distributor.h"

typedef struct player_vect{
	player_t** player;
	uint16_t alloced;
}players_t;

players_t* create_new_players(uint8_t num_players);

void game_get_nothing_card(dist_t* dist, players_t* players);

void game_get_cards(dist_t* dist, players_t* players);

uint8_t game_sum_voite(uint8_t* voites, uint16_t num);

void game_evolution(players_t* players, dist_t* dist);

void game_re_food(dist_t* dist, players_t* players);

void game_eat(dist_t* dist, players_t* players);

void game_kill_animals(players_t* players);

void game_delete_food(players_t* players);

void delete_players(players_t* players);
#endif
