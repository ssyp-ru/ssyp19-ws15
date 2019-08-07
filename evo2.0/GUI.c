#include <ncurses.h>
#include <stdlib.h>
#include "string.h"
#include "GUI.h"

GUI_t* GUI_init(uint16_t num_players){
	initscr();
	noecho();
	GUI_t* gui = (GUI_t*)malloc(sizeof(GUI_t));
	gui->players = (WINDOW**)malloc(sizeof(WINDOW*) * num_players);
	gui->num_players = num_players;
	int x, y;
	getmaxyx(stdscr, y, x);
	int tile_size = (x - 9) / num_players;
	gui->deck = newwin(y, 9, 0, 0);
	box(gui->deck, 0, 0);
	for (int i = 0; i < num_players; i++){
		gui->players[i] = newwin(y, tile_size, 0, (i * tile_size) + 9);
		box(gui->players[i], 0, 0);
		wrefresh(gui->players[i]);
	}
	wrefresh(gui->deck);
	refresh();
	return gui;
}

void GUI_information_redraw(GUI_t* gui, dist_t* dist, players_t* players){
	for (uint16_t i = 0; i < gui->num_players; i++){
		wclear(gui->players[i]);
		box(gui->players[i], 0, 0);
		wrefresh(gui->players[i]);
	}
	wclear(gui->deck);
	box(gui->deck, 0, 0);
	mvwprintw(gui->deck, 1, 1, "deck:%d", dist->len);
	wrefresh(gui->deck);
	for (uint16_t i = 0; i < gui->num_players; i++){
		uint8_t last_str = 1;
		mvwprintw(gui->players[i], last_str, 1, "Players N%d:", i + 1);
		last_str++;
		mvwprintw(gui->players[i], last_str, 1, "Cards in discard:%d", players->player[i]->trash->used);
		last_str++;
		animal_list_t* tmp = players->player[i]->head;
		mvwprintw(gui->players[i], last_str, 1, "Desk:");
		last_str++;
		for(int j = 1; tmp != NULL; j++){
			mvwprintw(gui->players[i], last_str, 2, "Animal N%d", j);
			last_str++;
			for (int k = 0; k < tmp->animal->used; k++){
				mvwprintw(gui->players[i], last_str, 3, "%s", tmp->animal->mods[k].name);
				last_str++;
			}
			tmp = tmp->next;
		}
		mvwprintw(gui->players[i], last_str, 1, "Hand:");
		last_str++;
		for(int j = 0; j < players->player[i]->hand->used; j++){
			if (players->player[i]->hand->hand[j].s_name != NULL){
				mvwprintw(gui->players[i], last_str, 2, "%s (%s)", players->player[i]->hand->hand[j].f_name, players->player[i]->hand->hand[j].s_name);
				last_str++;	
			}	else
				if (strcmp(players->player[i]->hand->hand[j].f_name, "nothing")){
					mvwprintw(gui->players[i], last_str, 2, "%s", players->player[i]->hand->hand[j].f_name);
					last_str++;
				}
		}
		wrefresh(gui->players[i]);
	}
}

void GUI_clear(GUI_t* gui){
	for (int i = 0; i < gui->num_players; i++)
		delwin(gui->players[i]);
	delwin(gui->deck);
	free(gui->players);
	free(gui);
	endwin();
}
