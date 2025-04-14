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
#include "../headers/client_processing.h"
#define MAX_INPUT 1024
#define MAX_TOKENS 16

void input_error(char *msg) {
    printf("Invalid Input: %s\n", msg);
}


//parser to figure out the command that the user sent
// parameters we need are the 
// 1. message
// 2. socket for passing it on to send stuff back
// we need to return an int to tell the client that the person has their name
// set or not. Returns 1 if the name gets set, returns 0 if the name is not
// set.
int tokenize_and_parse(char *input, int sock, int *name_set) {
    CommandType command;
    // check if input length is acceptable
    if (strlen(input) > 950) {
        input_error("Input too long");
        return -1;
    }
    // very simple logic, just use strtok to break all the crap down
    // Tokenize input (space-separated)
    char *token = strtok(input, " ");
    if (token == NULL) {
        input_error("Enter a command using /<command>");
        return -1;
    } 
    // Identify command
    if (strcmp(token, "/name") == 0) {
        command = CMD_SET_NAME;
    } else if (strcmp(token, "/disconnect") == 0
            || strcmp(token, "/quit") == 0) { 
        command = CMD_DISCONNECT;
    // if the user tries to do anything else without their name set, do not 
    // let them!
    } else if (!(*name_set)) {
        input_error("Please Enter your name using /name <name>");
        *name_set = 0;
        printf("FIRST TEST:  %d\n", *name_set);
        return -2;
    } else if (strcmp(token, "/list") == 0) {
        token = strtok(NULL, " ");
        printf("Token: %s\n", token);
        printf("Input: %s\n", input);
        if (strcmp(token, "people") == 0 && token != NULL) {
            command = CMD_LIST_PEOPLE;
        } else if (strcmp(token, "chats") == 0 && token != NULL) {
            command = CMD_LIST_CHATS;
        } else {
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
    } else {
        input_error("Enter a command using /<command>");
    }
    *name_set = 1;
    return command;
}
