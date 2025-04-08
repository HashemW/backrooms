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
#include "../headers/parser.h"


/*
 * Client main method, need two command line arguments
 * 1. IP of server
 * 2. Port
 */
int main(int argc, char *argv[]) {
    //parse client arguments
    int port = -1;
    char **ip_address;
    parse_client_args(argc, argv, port, ip_address)

 }
