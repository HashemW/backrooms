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
#define MAX_STRING_LENGTH 1024

/*
 * Code handles new connections connecting to the server. 
 *
 * Only parameter is the client socket.
 *
 */
void handle_new_connection(int sock) {
    char welcomeMessage[] = "Welcome to your secure chatting service\n"
                            "Please enter your name using /name <name>";
    int err = send(sock, welcomeMessage, strlen(welcomeMessage), 0);
    if (err < 0) {
        die("RECV() ERROR");
    }
}

/*
 * To handle an existing connection, we need to figure out what is being sent
 * then we handle it.
 * Only parameter we need is the client socket.
 * Return -1 if the user closes the connection
 */

int handle_existing_connection(int sock) {

} 

/*
 * We need to handle the case when the user joins and he needs to name 
 * himself.
 * only paramater we need is the client socket
 * Returns -1 if the user closes the connection
 */

int handle_no_name(int sock) {
    char welcomeMessage[] = "Please Enter \n"
                            "Please enter your name using /name <name>";
    int err = send(sock, welcomeMessage, strlen(welcomeMessage), 0);
    if (err < 0) {
        die("RECV() ERROR");
    }
}
