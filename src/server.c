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


// kill program function
void die(char* message) {
    fprintf(stderr, "%s: %s\n", message, strerror(errno));
    kill(0, SIGTERM);
    exit(errno);
}

/*
 * Here we set up our file descriptors
 */
int set_up_fd(int socket) {
    // check that our socket parameter is sane
    assert(socket > 0);
    // set up file descriptors to get multiple clients at once
    fd_set master_fds, read_fds;
    int max_fd = socket;

    FD_ZERO(&master_fds);
    FD_SET(socket, &master_fds);
    // exception to infinite loop rule, its meant to run forever.
    while(1) {
        // read the bluds
        read_fds = master_fds;
        if (select(max_fd + 1, &read_fds, NULL, NULL, NULL) == -1) {
            die("SELECT() ERROR");
        }
        // see if we are getting a new connection or a curr client
        for (int fd = 0; fd <= max_fd; fd++) {
            // if the curr is actually a connection
            if (FD_ISSET(fd, &read_fds)) {
                // this means its a new connection
                if (fd == socket) {
                    struct sockaddr_in client_address;
                    socklen_t client_address_size = sizeof(client_address);
                    // accept the new connection
                    int client_socket = accept(socket, 
                            (struct sockaddr *) &client_address, 
                            &client_address_size);
                    printf("New connection from %s on socket %d\n",
                            inet_ntoa(client_address.sin_addr), client_socket);
                    // add code to handle new connection
                    // handle_new_connection(client_socket);
                } else {
                    // add code to handle existing connection
                    // handle_existing_connection(client_socket)
                    // since this is a bit more complicated, we'll have to
                    // also check the return values
                }
            }
        }
    }
}
/*
 * Running the actual server. Only parameter we need is the port number.
 */
void run_server(int port) {
    //check that our port parameter is sane
    assert(port > 0);
    // initialize socket
    int server_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_socket < 0) {
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
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    // bind socket to an ip and port
    if (bind(server_socket, (struct sockaddr*) &server_address, 
                sizeof(server_address)) < 0) {
        die("BIND() ERROR");
    }
    // listen function pretty much says that we 
    // are ready to accept connections from a client
    if (listen(server_socket, 5) < 0) {
        die("LISTEN() ERROR");
    }
    //for a last time, check our socket is fine
    assert(server_socket > 0);
    // set up file descriptors to get multiple clients at once
    set_up_fd(server_socket);
}


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
    run_server(port);
    return 0;
}
