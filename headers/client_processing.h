#ifndef CLIENTPROC
#define CLIENTPROC

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/select.h>
#include <ctype.h>
#include "../headers/tools.h"


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


int tokenize_and_parse(char *input, int sock, int *name_set);

#endif
