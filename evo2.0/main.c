#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include "game.h"
#include "distributor.h"
#include "GUI.h"
#include "logic.h"

int main(){
	srand(time(NULL));
	dist_t* dist = create_new_distributor(4);
	distributor_load_weights(dist);
	players_t* players = create_new_players(4);
	GUI_t* gui = GUI_init(4);
	game_get_nothing_card(dist, players);
	game_get_cards(dist, players);
	while(dist->len > 0){
		game_evolution(players, dist);
		//logic_create_new_animal(players->player[0]);
		//logic_use_card(players, dist, 0);
		GUI_information_redraw(gui, dist, players);
		//logic_add_parasite_to_animal(players, dist, 0);
		//logic_create_new_animal(players->player[0]);
		//logic_animal_attack(players, dist, 0, players->player[0]->head->animal);
		game_re_food(dist, players);
		game_eat(dist, players);
		game_kill_animals(players);
		game_delete_food(players);
		game_get_cards(dist, players);
		getch();
		//char key = getch();
		//if (key == 'q')
		//	break;
	}
	game_evolution(players, dist);
	GUI_information_redraw(gui, dist, players);
	game_re_food(dist, players);
	game_eat(dist, players);
	game_kill_animals(players);
	game_delete_food(players);
	getch();
	delete_players(players);
	delete_distributor(dist);
	GUI_clear(gui);
	//_nc_free_and_exit();
	return 0;
}
