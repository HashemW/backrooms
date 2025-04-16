CC = gcc
CFLAGS = -Wall -g -Iheaders
SRC_DIR = src
OBJ_DIR = build
HASH_DIR = hashmap
BIN = server client

SERVERSOURCES = $(SRC_DIR)/server.c $(SRC_DIR)/tools.c $(SRC_DIR)/server_processing.c $(SRC_DIR)/client_processing.c
CLIENTSOURCES = $(SRC_DIR)/client.c $(SRC_DIR)/tools.c $(SRC_DIR)/client_processing.c
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))

all: $(BIN)


server: $(SERVERSOURCES)
	$(CC) $(CFLAGS) -o $@ $^
client: $(CLIENTSOURCES)
	$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c headers/tools.h headers/server_processing.h headers/client_processing.h | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -rf $(BIN) $(OBJ_DIR)

