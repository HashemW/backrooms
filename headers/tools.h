#ifndef TOOLS
#define TOOLS
#include <signal.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

void die(char* message);

void parse_server(int argc, char *argv[], int *port);

void parse_client_args(int argc, char *argv[], char **ip_address, int *port);

#endif
