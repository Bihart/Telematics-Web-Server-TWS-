#ifndef SERVER_H
#define SERVER_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <pthread.h>

int create_socket();
void setup_server(int server_socket, int PORT);
void *handle_request(void *client_socket);
void handle_connection(int server_socket);

#endif