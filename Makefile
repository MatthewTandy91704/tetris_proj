CC = gcc
CFLAGS = -Wall -Werror -std=c17

tetris: tetris.o
		$(CC) $(CFLAGS) -IC:/Users/mt917/coding_projects/tetris_proj/libs/SDL2-2.30.11/include -c tetris.c -o tetris.o

tetris.o: tetris.c
		$(CC) $(CFLAGS) tetris.o -o tetris -LC:/Users/mt917/coding_projects/tetris_proj/libs/SDL2-2.30.11/x86_64-w64-mingw32/lib -lSDL2main -lSDL2 -mconsole
