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
#include "../headers/client_processing.h"
#define MAX_INPUT 1024

typedef struct message {
    CommandType command;

} msg;
void client_loop(int sock) {
    char input[MAX_INPUT];
    int name_set = 0;
    CommandType command;
    while (1) {
        printf("> ");
        fflush(stdout); // Make sure the prompt prints immediately

        if (fgets(input, sizeof(input), stdin) != NULL) {
            // Remove newline
            input[strcspn(input, "\n")] = 0;
            command = tokenize_and_parse(input, sock, &name_set);
            printf("Name set: %d\n", name_set); 
        } else {
            // Handle EOF (Ctrl+D) or error
            printf("Input error or EOF\n");
            break;
        }
    }
}
/*
 * Run client method with two parameters
 * 1. IP address of server
 * 2. Port
 */
void run_client(char *ip_address, int port) {
    assert(port > 0);
    assert(ip_address != NULL);
    int client_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (client_socket < 0) {
        die("SOCKET() ERROR");
    }
    //make address structure
    struct sockaddr_in server_address;
    // clear memory to 0s.
    memset(&server_address, 0, sizeof(server_address));
    // set to IPv4
    server_address.sin_family = AF_INET;
    // htons -> host to network short
    server_address.sin_port = htons(port);
    // INADDR_ANY means that connections are accepted
    // on all available network interfaces
    if (inet_pton(AF_INET, ip_address, &server_address.sin_addr) <= 0) {
        die("INVALID ADDRESS");
    }
    //connect to server
    if (connect(client_socket, (struct sockaddr*)&server_address, 
                sizeof(server_address)) < 0) {
        die("CONNECT() ERROR");
    }
    char buffer[MAX_INPUT];
    if (recv(client_socket, buffer, MAX_INPUT, 0) < 0) {
        die("RECV() ERROR");
    }
    printf("%s\n", buffer);
    client_loop(client_socket); 
}
/*
 * Client main method, need two command line arguments
 * 1. IP of server (should be 127.0.0.1 for local)
 * 2. Port
 */
int main(int argc, char *argv[]) {
    //parse client arguments
    int port = -1;
    char *ip_address = NULL;
    parse_client_args(argc, argv, &ip_address, &port);
    assert(port != -1);
    assert(ip_address != NULL);
    printf("IP Address: %s, port Number: %d\n", ip_address, port);
    srand(time(NULL));
    run_client(ip_address, port); 

    return 0;
 }
