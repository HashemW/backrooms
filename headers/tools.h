#ifndef TOOLS
#define TOOLS
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#define MAX_INPUT 1024
typedef enum {
    CMD_UNKNOWN,
    CMD_LIST_PEOPLE,
    CMD_LIST_CHATS,
    CMD_JOIN,
    CMD_CREATE,
    CMD_LEAVE,
    CMD_MSG,
    CMD_DISCONNECT,
    CMD_SET_NAME
} CommandType;

typedef struct message {
    CommandType command;
    char arg1[MAX_INPUT];
    char arg2[MAX_INPUT];
} msg;

void die(char* message);

void parse_server(int argc, char *argv[], int *port);

void parse_client_args(int argc, char *argv[], char **ip_address, int *port);

#endif
