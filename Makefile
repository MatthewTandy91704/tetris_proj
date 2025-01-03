CC = gcc

tetris: tetris.o
		$(CC) -Wall -Werror -std=c17 tetris.o -o tetris

tetris.o: tetris.c
		$(CC) -Wall -Werror -std=c17 -c tetris.c -o tetris.o
