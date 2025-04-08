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
#include "parser.h"
#include "eventhandlers.h"


int main(int argc, char *argv[]) {
    struct server_arguments args;
    args = server_parseopt(argc, argv);
    printf("Port Number: %d\n", args.port);
    srand(time(NULL));
    //run_server(args.port);
    return 0;
}
