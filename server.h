#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "request.h"


struct handle_request_args {
    int *client_socket;
    // nuevo argumento
    const char *PATH;
    const char *LOGFILE;
};

int create_socket();
void setup_server(int server_socket, int PORT);
void *handle_request(void *client_socket_args);
void handle_connection(int server_socket, const char *PATH, const char *LOGFILE);

#endif