CC = gcc
CFLAGS = -Wall -g -Iheaders
SRC_DIR = src
OBJ_DIR = build
BIN = server

SOURCES = $(SRC_DIR)/server.c $(SRC_DIR)/parser.c
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

all: $(BIN)

$(BIN): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c headers/parser.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BIN) $(OBJ_DIR)

