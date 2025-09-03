flags = -Wall
links = -lncurses

all:
	gcc tetris.c $(flags) $(links) -o tetris

run:
	./tetris
