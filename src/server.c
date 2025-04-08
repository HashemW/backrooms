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
 * Main method, we need one server argument to run this method
 * 1. Port
 */
int main(int argc, char *argv[]) {
    // parse server arguments and find the port.
    int port = -1;
    parse_server(argc, argv, &port);
    assert(port != 1);
    printf("Port Number: %d\n", port);
    srand(time(NULL));
    //run_server(args.port);
    return 0;
}
