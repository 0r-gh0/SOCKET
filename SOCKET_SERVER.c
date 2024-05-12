#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define PORT 9999
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configuring server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    // Binding socket to the address
    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Binding failed");
        exit(EXIT_FAILURE);
    }

    // Listening for incoming connections
    if (listen(server_fd, 5) == -1) {
        perror("Listening failed");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d\n", PORT);

    // Accepting incoming connections
    if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
        perror("Accepting connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to client\n");

    // Receiving and sending data
    while (1) {
        // Receiving from client
        memset(buffer, 0, sizeof(buffer));
        if (recv(client_fd, buffer, sizeof(buffer), 0) == -1) {
            perror("Receiving data failed");
            exit(EXIT_FAILURE);
        }
        printf("Received from client: %s\n", buffer);

        // Sending back to client
        if (send(client_fd, buffer, strlen(buffer), 0) == -1) {
            perror("Sending data failed");
            exit(EXIT_FAILURE);
        }
    }

    close(server_fd);
    return 0;
}

