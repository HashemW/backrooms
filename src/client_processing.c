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

#define MAX_INPUT 1024
#define MAX_TOKENS 16

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

void input_error(char *msg) {
    printf("Invalid Input: %s\n", msg);

}


//parser to figure out the command that the user sent
// parameters we need are the 
// 1. message
// 2. socket for passing it on to send stuff back
void tokenize_and_parse(char *input, int sock) {
    CommandType command;
    // very simple logic, just use strtok to break all the crap down
    // Tokenize input (space-separated)
    char *token = strtok(input, " ");
    if (token == NULL) {
        input_error("Enter a command using /<command>");
        return;
    } 
    printf("%s\n", token);
    // Identify command
    if (strcmp(token, "/list") == 0) {
        token = strtok(input, " ");
        if (strcmp(token, "people") == 0)
            command = CMD_LIST_PEOPLE;
        else if (strcmp(token, "chats") == 0)
            command = CMD_LIST_CHATS;
        else {
            input_error("Use /list <command>");
        }
    } else if (strcmp(token, "/join") == 0) {
        command = CMD_JOIN;
    } else if (strcmp(token, "/create") == 0) {
        command = CMD_CREATE;
    } else if (strcmp(token, "/leave") == 0) {
        command = CMD_LEAVE;
    } else if (strcmp(token, "msg") == 0) {
        command = CMD_MSG;
    } else if (strcmp(token, "/disconnect") == 0 
            || strcmp(token, "/quit") == 0) {
        command = CMD_DISCONNECT;
    } else if (strcmp(token, "/name") == 0) {
        command = CMD_SET_NAME;
    } else {
        input_error("Enter a command using /<command>");
    }

    printf("%d\n", command);
}
