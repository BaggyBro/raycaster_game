
CC = gcc
CFLAGS = -Wall -Wextra -Iinclude $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs) -lGL -lGLU -lGLU -lm
SRC = src/main.c src/game.c
OBJ = $(SRC:.c=.o)
EXEC = game

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CC) $(OBJ) -o $(EXEC) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(EXEC)

run: $(EXEC)
	./$(EXEC)
