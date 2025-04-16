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
#include "../headers/server_processing.h"
#include "../headers/client_processing.h"
#define MSG_SIZE 2052
/*
 * Code handles new connections connecting to the server. 
 *
 * Only parameter is the client socket.
 *
 */
void handle_new_connection(int sock, user *users) {
    char welcomeMessage[] = "Welcome to your secure chatting service\n"
                            "Please enter your name using /name <name>";
    int err = send(sock, welcomeMessage, strlen(welcomeMessage), 0);
    if (err < 0) {
        die("SEND() ERROR");
    }
    for (int i = 0; i < MAX_USERS; i++) {
        if (users[i].sock == 0) {
            // # means the name is unset
            users[i].name_set = 0;
            users[i].sock = sock;
            break;
        }
    }
}
/*
* This function is used to send a failure message to the client.
* It takes the socket and the message to be sent as parameters.
* The message is sent using the send() function.
* If the send() function fails, it calls the die() function to handle the error.
*/
void send_failure(int sock) {
    // send the failure message
    network_msg failure;
    failure.command = CMD_UNKNOWN;
    int err = send(sock, &failure, sizeof(failure), 0);
    if (err < 0) {
        die("SEND() ERROR");
    }
}
/*
Send to the client that the server has received the message
* This is a simple confirmation message.
*/
void send_confirmation(int sock, CommandType command, char *message) {
    network_msg confirmation;
    confirmation.command = command;
    strcpy(confirmation.arg1, message);
    int err = send(sock, &confirmation, sizeof(confirmation), 0);
    if (err < 0) {
        die("SEND() ERROR");
    }
    memset(confirmation.arg1, 0, sizeof(confirmation.arg1));
}
/*
 * To handle an existing connection, we need to figure out what is being sent
 * then we handle it.
 * Only parameter we need is the client socket.
 * Return -1 if the user closes the connection
 */

int handle_existing_connection(int sock, user *usr) {
    msg recevition;
    int status = recv(sock, &recevition, MSG_SIZE, 0);
    if (status == 0) {
        return -1;
    } else if (status < 0) {
        die("RECV() ERROR");
    }
    // do basic command split up.

    return 0;
} 

void handle_connection(int sock, user *usr, char* input) {
    // check if the user has a name set
    msg newMsg;
    // if there are arguments newMsg.input is passed in to take it
    CommandType command = tokenize_and_parse(input, sock, &(usr->name_set), 
        newMsg.arg1, newMsg.arg2);
    printf("Command: %d\n", command);
    printf("Argument 1: %s\n", newMsg.arg1);
    printf("Argument 2: %s\n", newMsg.arg2);
    if (command == -1) {
        send_failure(sock);
    } else if (command == CMD_SET_NAME) {
        strcpy(usr->name, newMsg.arg1);
        usr->name_set = 1;
        send_confirmation(sock, command, newMsg.arg1);
    } else if (command == CMD_LIST_PEOPLE) {
        send_confirmation(sock, command, "List of people");
    } else if (command == CMD_LIST_CHATS) {
        send_confirmation(sock, command, "List of chats");
    } else if (command == CMD_JOIN) {
        send_confirmation(sock, command, "Join chat");
    } else if (command == CMD_CREATE) {
        send_confirmation(sock, command, "Create chat");
    } else if (command == CMD_LEAVE) {
        send_confirmation(sock, command, "Leave chat");
    } else if (command == CMD_MSG) {
        send_confirmation(sock, command, "Message sent");
    } else if (command == CMD_DISCONNECT) {
        send_confirmation(sock, command, "Disconnected");
    }
    memset(newMsg.arg1, 0, sizeof(newMsg.arg1));
    memset(newMsg.arg2, 0, sizeof(newMsg.arg2));
}
/*
 * We need to handle the case when the user joins and he needs to name 
 * himself.
 * only paramater we need is the client socket
 * Returns -1 if the user closes the connection
 */

int handle_no_name(int sock, user *usr) {
    msg recevition;
    int status = recv(sock, &recevition, MSG_SIZE, 0);
    if (status == 0) {
        return -1;
    } else if (status < 0) {
        die("RECV() ERROR");
    }
    strcpy(usr->name, recevition.arg1);
    return 0;
}
