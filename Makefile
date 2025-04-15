CC = gcc
CFLAGS = -Wall -std=c99 -Iincludes/
LDFLAGS = -lsqlite3

SRC = src/main.c src/auth.c src/database.c src/country.c src/region.c
OBJ = $(SRC:.c=.o)
BIN = bin/country_app

all: $(BIN)

$(BIN): $(OBJ)
	mkdir -p bin
	$(CC) $(CFLAGS) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(BIN)

.PHONY: all clean
