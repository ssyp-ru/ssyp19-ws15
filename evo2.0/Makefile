CC=gcc
CFLAGS=-g -c -Wall -Wextra -Werror -Wpedantic -fstrict-aliasing -std=c11
LDFLAGS= -lncurses
SOURCES=animal.c player.c distributor.c GUI.c game.c hand.c find.c logic.c main.c
OBJECTS=$(SOURCES:.c=.o)
EXECUTABLE=main

all:	$(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -lncurses -o $(EXECUTABLE)

.c.o:
	$(CC) $(CFLAGS)  $< -lncurses -o $@

clean: 
	rm -rf *.o 
