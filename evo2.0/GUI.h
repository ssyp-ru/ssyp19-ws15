#ifndef _06_07_GUI
#define _06_07_GUI

#include <ncurses.h>
#include "distributor.h"
#include "game.h"

typedef struct GUI{
	WINDOW** players;
	WINDOW* deck;
	uint16_t num_players;
}GUI_t;

GUI_t* GUI_init(uint16_t num_players);

void GUI_information_redraw(GUI_t* gui, dist_t* dist, players_t* players);

void GUI_clear(GUI_t* gui);

#endif
