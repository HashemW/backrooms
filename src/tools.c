#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <assert.h>
#define MAX_STRING_LENGTH 512
#define MAX_GETOPT_LOOPS 10

//simple, parse server arguments and get the port number
void parse_server(int argc, char *argv[], int *port) {
    int opt;
    int loop_count = 0;
    assert(*port == -1);
    while ((opt = getopt(argc, argv, "p:")) != -1) {
        loop_count++;
        assert(loop_count <= MAX_GETOPT_LOOPS);  //Enforce loop iteration limit

        switch (opt) {
            case 'p':  // Port
                *port = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -p <port>\n",argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (*port == -1) {
        fprintf(stderr, "Missing required arguments.\nUsage: %s -p <port>\n", 
                argv[0]);
        exit(EXIT_FAILURE);
    }
}

// simple, parse the client arguments get the port and IP address
void parse_client_args(int argc, char *argv[], char **ip_address, int *port) {
    int opt;
    int loop_count = 0;
    assert((*port) == -1);
    assert(*ip_address == NULL);
    while ((opt = getopt(argc, argv, "a:p:")) != -1) {
        loop_count++;
        assert(loop_count <= MAX_GETOPT_LOOPS);  //Enforce loop iteration limit

        switch (opt) {
            case 'a':  // IP address
                *ip_address = optarg;
                break;
            case 'p':  // Port
                *port = atoi(optarg);
                break;
            default:
                fprintf(stderr, "Usage: %s -a <ip_address> -p <port>\n", 
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }

    if (*ip_address == NULL || *port == -1) {
        fprintf(stderr, "Missing required arguments.\nUsage: %s -a" 
                "<ip_address> -p <port>\n", argv[0]);
        exit(EXIT_FAILURE);
    }
}

void die(char* message) {
    fprintf(stderr, "%s: %s\n", message, strerror(errno));
    kill(0, SIGTERM);
    exit(errno);
}

