#ifndef SERVERPROC
#define SERVERPROC
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
#define MAX_USERS 128

void handle_new_connection(int sock, user *usr);

int handle_existing_connection(int sock, user *usr);

int handle_no_name(int sock, user *usr);

void handle_connection(int sock, user *users, user *usr, char* input);

#endif
