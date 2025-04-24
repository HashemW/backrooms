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
void send_failure(int sock, char *message) {
    // send the failure message
    network_msg failure;
    failure.command = CMD_UNKNOWN;
    strcpy(failure.arg1, message);
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

void handle_connection(int sock, user *users, chat_room *rooms, user *usr, 
    char* input) {
    // check if the user has a name set

    msg newMsg;
    char err_msg[MAX_INPUT];
    // if there are arguments newMsg.input is passed in to take it
    CommandType command = tokenize_and_parse(input, sock, &(usr->name_set), 
        newMsg.arg1, newMsg.arg2, err_msg);
    printf("Command: %d\n", command);
    printf("Argument 1: %s\n", newMsg.arg1);
    printf("Argument 2: %s\n", newMsg.arg2);
    if (command == -1) {
        send_failure(sock, err_msg);
    } else if (command == CMD_SET_NAME) {
        strcpy(usr->name, newMsg.arg1);
        usr->name_set = 1;
        send_confirmation(sock, command, newMsg.arg1);
    } else if (command == CMD_LIST_PEOPLE) {
        // two options, if someone is in a chatroom, or not
        char peopleList[MAX_INPUT];
        if (usr->in_room == 0) {
            for (int i = 0; i < MAX_USERS; i++) {
                if (users[i].sock != 0) {
                    strcat(peopleList, users[i].name);
                    strcat(peopleList, "\n");
                }
            }
        } else {
            // this code will need to be debugged.
            for (int i = 0; i < MAX_ROOMS; i++) {
                
                if (rooms[i].in_use && 
                        strcmp(usr->curr_room, rooms[i].name) == 0) {
                    for (int j = 0; j < MAX_USERS; j++) {
                        if (rooms[i].users[j]->sock != 0) {
                            strcat(peopleList, rooms[i].users[j]->name);
                            strcat(peopleList, "\n");
                            printf("People list: %s\n", peopleList);
                            break;
                        }
                    }
                }
            }
        }
        send_confirmation(sock, command, peopleList);
        memset(peopleList, 0, sizeof(peopleList));
    } else if (command == CMD_LIST_CHATS) {
        char chatList[MAX_INPUT];
        for (int i = 0; i < MAX_ROOMS; i++) {
            if (rooms[i].in_use) {
                strcat(chatList, rooms[i].name);
                strcat(chatList, "\n");
            }
        }
        send_confirmation(sock, command, chatList);
        memset(chatList, 0, sizeof(chatList));
    } else if (command == CMD_JOIN) {
        // join chat process is easy, check if chat exists, if it doesnt
        // create it, else join it
        // first, if the user is in a room, we need to purge that.
        printf("Ruinning join\n");
        if (usr->in_room == 1) {
            for (int i = 0; i < MAX_ROOMS; i++) {
                if (rooms[i].in_use && strcmp(usr->curr_room, rooms[i].name) == 0) {
                    // remove the user from the room
                    for (int j = 0; j < MAX_USERS; j++) {
                        if (rooms[i].users[j]->sock == usr->sock) {
                            // remove the user from the room
                            rooms[i].users[j]->sock = 0;
                            memset(rooms[i].users[j]->name, 0, 
                                sizeof(rooms[i].users[j]->name));
                            memset(rooms[i].users[j]->curr_room, 0,
                                sizeof(rooms[i].users[j]->curr_room));
                            usr->in_room = 0;
                            strcpy(usr->curr_room, "");
                            // remove the user from the room
                            rooms[i].population--;
                            //check if the room is empty, if it is delete
                            if (rooms[i].population == 0) {
                                rooms[i].in_use = 0;
                                memset(rooms[i].name, 0, sizeof(rooms[i].name));
                                memset(rooms[i].password, 0, 
                                    sizeof(rooms[i].password));
                            }
                            break;
                        }
                    }
                } 
            }
        }
        printf("Ruinning join2\n");
        for (int i = 0; i < MAX_ROOMS; i++) {
            if (rooms[i].in_use && strcmp(rooms[i].name, newMsg.arg1) == 0) {
                // add the user to the room
                if (strcmp(rooms[i].password, newMsg.arg2) == 0) {
                    for (int j = 0; j < MAX_USERS; j++) {
                        if (rooms[i].users[j]->sock == 0) {
                            rooms[i].users[j] = usr;
                            usr->in_room = 1;
                            strcpy(usr->curr_room, rooms[i].name);
                            rooms[i].population++;
                            break;
                        }
                    }
                } else {
                    send_failure(sock, "Incorrect password");
                    memset(newMsg.arg1, 0, sizeof(newMsg.arg1));
                    memset(newMsg.arg2, 0, sizeof(newMsg.arg2));
                    memset(err_msg, 0, sizeof(err_msg));
                    return;
                }
            }
        }
        printf("Ruinning join4\n");
        if (usr->in_room == 0) {
            // create the room
            for (int i = 0; i < MAX_ROOMS; i++) {
                if (rooms[i].in_use == 0) {
                    rooms[i].in_use = 1;
                    strcpy(rooms[i].name, newMsg.arg1);
                    strcpy(rooms[i].password, newMsg.arg2);
                    rooms[i].users[0] = usr;
                    usr->in_room = 1;
                    strcpy(usr->curr_room, rooms[i].name);
                    strcpy(rooms[i].users[0]->name, usr->name);
                    rooms[i].population++;
                    break;
                }
            }
        }
        send_confirmation(sock, command, newMsg.arg1);
    } else if (command == CMD_LEAVE) {
        send_confirmation(sock, command, "Left chat!");
    } else if (command == CMD_MSG) {
        send_confirmation(sock, command, "Message sent");
    } else if (command == CMD_DISCONNECT) {
        send_confirmation(sock, command, "Disconnected");
    } else {
        send_failure(sock, "Unknown command");
    }
    memset(newMsg.arg1, 0, sizeof(newMsg.arg1));
    memset(newMsg.arg2, 0, sizeof(newMsg.arg2));
    memset(err_msg, 0, sizeof(err_msg));
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
