#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

void *receive_messages(void *socket_desc) {
    int sock = *(int *)socket_desc;
    char buffer[BUFFER_SIZE];
    int bytes_received; 

    while ((bytes_received = recv(sock, buffer, BUFFER_SIZE, 0)) > 0) {
        buffer[bytes_received] = '\0';  // Null-terminate the string
        printf("\nReceived: %s\n", buffer);  // Display the received message
        printf("Enter message: ");  // Prompt the user to enter a reply
        fflush(stdout);  // Force output to terminal
    }

    if (bytes_received == 0) {
        printf("Server disconnected.\n");
    } else if (bytes_received == -1) {
        perror("recv failed");
    }

    return NULL;
}

int main() {
    int sock;
    struct sockaddr_in server;
    char message[BUFFER_SIZE];
    pthread_t recv_thread;

    // Create socket
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        printf("Could not create socket");
        return 1;
    }

    // Define server address
    server.sin_addr.s_addr = inet_addr("127.0.0.1");
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    // Connect to server
    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connect failed. Error");
        return 1;
    }

    printf("Connected to server.\n");

    // Start receiving messages in a separate thread
    if (pthread_create(&recv_thread, NULL, receive_messages, (void *)&sock) < 0) {
        perror("Could not create thread");
        return 1;
    }

    // Main loop for sending messages
    while (1) {
        printf("Enter message: ");
        fgets(message, BUFFER_SIZE, stdin);
        message[strcspn(message, "\n")] = '\0';  // Remove newline character

        // Send message to server
        if (send(sock, message, strlen(message), 0) < 0) {
            printf("Send failed\n");
            return 1;
        }
    }

    close(sock);
    return 0;
}
