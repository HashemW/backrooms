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
#define MAX_STRING_LENGTH 512


/*
 * Code handles new connections connecting to the server. 
 *
 * Only parameter is the client socket.
 *
 * If something goes wrong or an error of sorts happens, or the user does 
 * something stupid, it will return -1, telling the server to shut this 
 * connection. Otherwise, good things happen!
 */
int handle_new_connection(int sock) {
    char welcomeMessage[] = "Welcome to your secure chatting service";
    int err = send(sock, welcomeMessage, strlen(welcomeMessage), 0);
    if (err < 0) {
        die("RECV() ERROR");
    }
    return 0;
}
