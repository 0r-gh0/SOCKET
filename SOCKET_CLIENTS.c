#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>

#define SERVER_IP "127.0.0.1"
#define PORT 6969
#define BUFFER_SIZE 1024

int main() {
    int client_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];

    // Creating socket file descriptor
    if ((client_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configuring server address
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(SERVER_IP);
    server_addr.sin_port = htons(PORT);

    // Connecting to server
    if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }

    printf("Connected to server\n");

    // Sending and receiving data
    while (1) {
        // Sending to server
        printf("Enter message to send: ");
        fgets(buffer, BUFFER_SIZE, stdin);
        
        // Check if the input is -1 to break the loop and disconnect
        if (strcmp(buffer, "-1") == 0) {
            printf("Disconnecting from the server...\n");
            break;
        }
        
        if (send(client_fd, buffer, strlen(buffer), 0) == -1) {
            perror("Sending data failed");
            exit(EXIT_FAILURE);
        }

        // Receiving from server
        memset(buffer, 0, sizeof(buffer));
        if (recv(client_fd, buffer, sizeof(buffer), 0) == -1) {
            perror("Receiving data failed");
            exit(EXIT_FAILURE);
        }
        printf("Received from server: %s\n", buffer);
    }

    close(client_fd);
    printf("Disconnected from server\n");
    return 0;
}
