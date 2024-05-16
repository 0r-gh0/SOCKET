#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 9999
#define BUFFER_SIZE 1024

// Function to handle each client connection
void *handle_client(void *arg) {
    int client_fd = *((int *)arg);
    char buffer[BUFFER_SIZE];

    while (1) {
        // Receiving from client
        memset(buffer, 0, sizeof(buffer));
        if (recv(client_fd, buffer, sizeof(buffer), 0) == -1) {
            perror("Receiving data failed");
            close(client_fd);
            pthread_exit(NULL);
        }
        
        // printf("Received from client %d: %s\n", client_fd, buffer);
        // Convert received message to integer
        int num = atoi(buffer);
        printf("Received from client %d: %d\n", client_fd, num);
        
        // If client sends 0, stop working and remove the user
        if (num == 0) {
            printf("Client %d sent 0, closing connection.\n", client_fd);
            close(client_fd);
            pthread_exit(NULL);
        }
        
        // Add 5 to the received number
        num += 5;
        sprintf(buffer, "%d", num);

        // Sending back to client
        if (send(client_fd, buffer, strlen(buffer), 0) == -1) {
            perror("Sending data failed");
            close(client_fd);
            pthread_exit(NULL);
        }
    }
    close(client_fd);
    pthread_exit(NULL);
}

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);
    char buffer[BUFFER_SIZE];
    pthread_t tid;

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

    // Accepting incoming connections and creating threads
    while (1) {
        // Accepting incoming connection
        if ((client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len)) == -1) {
            perror("Accepting connection failed");
            continue;
        }

        printf("Connected to client %d\n", client_fd);

        // Creating a thread for each client
        if (pthread_create(&tid, NULL, handle_client, &client_fd) != 0) {
            perror("Thread creation failed");
            close(client_fd);
            continue;
        }
    }

    close(server_fd);
    return 0;
}
