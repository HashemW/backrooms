#ifndef PARSER
#define PARSER

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>

void parse_server(int argc, char *argv[], int *port);
void parse_client_args(int argc, char *argv[], char **ip_address, int *port);

#endif
