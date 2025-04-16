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

/*
 * Just to make the code cleaner made this function to parse between which
 * list the user wants
 */
int deal_with_list(char *token, char *err_msg) {
    CommandType command;
    token = strtok(NULL, " ");
    if (strcmp(token, "people") == 0 && token != NULL) {
        command = CMD_LIST_PEOPLE;
    } else if (strcmp(token, "chats") == 0 && token != NULL) {
        command = CMD_LIST_CHATS;
    } else {
        strcpy(err_msg, "Use /list <command>");
        return -1;
    }
    return command;
}

/*
 * This is for dealing with two arguments.
 * we parse and give the user back the arguments (we have a most of two
 * arguments)
 * the two parameters are strings for the arguments
 */
int deal_with_rooms(char *token, char *arg1, char *arg2, char *err_msg) {
    //name
    token = strtok(NULL, " ");
    if (token == NULL) {
        strcpy(err_msg, "This command requires an argument!");
        return -1;
    }
    strcpy(arg1, token);
    //password
    token = strtok(NULL, " ");
    if (token == NULL) {
        arg2 = NULL;
    } else {
        strcpy(arg2, token);
    }
    return 0;
}

char *get_arguments(char *msg) {
    char *space_ptr = strchr(msg, ' ');
    char *substring;
    //printf("SUBSTRING: %s\n", msg);
    if (space_ptr != NULL) {
        substring = space_ptr + 1;
    } else {
        return NULL;
    }
    return substring;
}

/*
 * Deal with message stuff and arguments
 */
int deal_with_msg(char *token, char *input, char *arg1, char *arg2, 
    char *err_msg) {
    // name
    token = strtok(NULL, " ");
    if (token == NULL) {
        strcpy(err_msg, "This command requires two arguments!");
        return -1;
    }
    strcpy(arg1, token);
    //message
    //check if there are two arguments
    token = strtok(NULL, " ");
    if (token == NULL) {
        strcpy(err_msg, "This command requires two arguments!");
        return -1;
    }
    char *substring = get_arguments(input);
    substring = get_arguments(substring);
    strcpy(arg2, substring);
    return 0;
}

//parser to figure out the command that the user sent
// parameters we need are the 
// 1. message
// 2. socket for passing it on to send stuff back
// we need to return an int to tell the client that the person has their name
// set or not. Returns 1 if the name gets set, returns 0 if the name is not
// set.
int tokenize_and_parse(char *input, int sock, int *name_set, char *arg1, 
        char *arg2, char *err_msg) {
    CommandType command;
    // check if input length is acceptable
    if (strlen(input) > 950) {
        strcpy(err_msg, "Input too long");
        return -1;
    }
    char inputCopy[MAX_INPUT];
    strcpy(inputCopy, input);
    // very simple logic, just use strtok to break all the crap down
    // Tokenize input (space-separated)
    char *token = strtok(input, " ");
    if (token == NULL) {
        strcpy(err_msg, "Enter a command using /<command>");
        return -1;
    } 
    // Identify command
    if (strcmp(token, "/name") == 0) {
        command = CMD_SET_NAME;
        deal_with_rooms(token, arg1, arg2, err_msg);
    } else if (strcmp(token, "/disconnect") == 0
            || strcmp(token, "/quit") == 0) { 
        command = CMD_DISCONNECT;
    // if the user tries to do anything else without their name set, do not 
    // let them!
    } else if (!(*name_set)) {
        strcpy(err_msg, "Please Enter your name using /name <name>");
        *name_set = 0;
        return -1;
    } else if (strcmp(token, "/list") == 0) {
        command = deal_with_list(token, err_msg);
    } else if (strcmp(token, "/leave") == 0) {
        command = CMD_LEAVE;
    // deal with multi argument requests
    } else {
        int dealing;
        if (strcmp(token, "/join") == 0) {
            command = CMD_JOIN;
            dealing = deal_with_rooms(token, arg1, arg2, err_msg);
        } else if (strcmp(token, "/create") == 0) {
            command = CMD_CREATE;
            dealing = deal_with_rooms(token, arg1, arg2, err_msg);
        } else if (strcmp(token, "/msg") == 0) {
            command = CMD_MSG;
            dealing = deal_with_msg(token, inputCopy, arg1, arg2, err_msg);
        } else {
            strcpy(err_msg, "Enter a command using /<command>");
            return -1;
        }
        if (dealing == -1) {
            return -1;
        }
    }
    *name_set = 1;
    memset(inputCopy, 0, sizeof(inputCopy));
    return command;
}
