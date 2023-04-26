#include "server.h"

int create_socket() {
    int server_socket;
    if ((server_socket = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    return server_socket;
}

void setup_server(int server_socket, int PORT) {
    struct sockaddr_in address;
    
    int opt = 1;
    if (setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt failed");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_socket, 3) < 0) {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
}

void *handle_request(void *client_socket) {
    int _client_socket = *(int *) client_socket; //client socket
    char buffer[1024];
    int valread;
    valread = read(_client_socket, buffer, 1024);
    if (valread < 1) {
        perror("No data.");
        return NULL;
    }
    printf("Received:\n%s", buffer); //print all request
    send(_client_socket, buffer, strlen(buffer), 0);
    memset(buffer, 0, sizeof(buffer));
    close(_client_socket);
    return NULL;
}

void handle_connection(int server_socket){
    int newSocket;
	struct sockaddr_in newAddr;
    socklen_t address_length;

	while(1){
		newSocket = accept(server_socket, (struct sockaddr *) &newAddr, &address_length);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));
        
        pthread_t threadid;
        if (pthread_create(&threadid, NULL, handle_request, (void *) &newSocket) < 0) {
            perror("Error: could not create thread");
            exit(1);
        }
	}
}