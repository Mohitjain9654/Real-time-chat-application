#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define PORT 8080
#define BUFFER_SIZE 1024
#define USERNAME_SIZE 50

//lobal Functions
void *receive_messages(void *sock_desc);

int main() {
    int sock;
    struct sockaddr_in server;
    char message[BUFFER_SIZE];
    char username[USERNAME_SIZE];
    pthread_t recv_thread;

    // Create socket

    //The program creates a socket using the socket() function with the AF_INET address family (IPv4) and SOCK_STREAM type (TCP).

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        
        //If the socket creation fails, it prints an error message and exits.

        printf("Could not create socket\n");
        return 1;
    }

    // Set the server IP address here (replace with the server's IP)

    //The server.sin_addr.s_addr is set to the IP address of the server (here 192.168.1.37, which can be modified).

    server.sin_addr.s_addr = inet_addr("192.168.253.2");
    server.sin_family = AF_INET;

    //The server.sin_family is set to AF_INET for IPv4, and server.sin_port is set to the server's port number (8080) using htons() to ensure the correct byte order for the port

    server.sin_port = htons(PORT);

    // Connect to server

    //The program attempts to connect to the server using connect(). If the connection fails, an error message is printed, and the program exits.

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connection failed. Error");
        return 1;
    }
    printf("Connected to the server.\n");

    // Prompt for username

    //The program prompts the user to enter a username. 

    printf("Enter your username: ");
    fgets(username, USERNAME_SIZE, stdin);
    username[strlen(username) - 1] = '\0';  // Remove newline character

    // Create a thread to receive messages from the server

    //The program creates a separate thread using pthread_create() to handle receiving messages from the server. The receive_messages() function is passed the socket descriptor (sock) as an argument.

    if (pthread_create(&recv_thread, NULL, receive_messages, (void *)&sock) < 0) {
        perror("Could not create thread for receiving messages");
        return 1;
    }

    // Keep sending messages to the server

    //The main thread enters a loop where it continuously prompts the user to enter messages.

    while (1) {
        char message[BUFFER_SIZE - USERNAME_SIZE];
        printf("Enter message: ");
        fgets(message, sizeof(message), stdin);
        message[strlen(message) - 1] = '\0';  // Remove newline character

        // Format message with username
        char formatted_message[BUFFER_SIZE];
        snprintf(formatted_message, BUFFER_SIZE, "%s: %s", username, message);

        // Send the message to the server

        // sent to the server

        if (send(sock, formatted_message, strlen(formatted_message), 0) < 0) {
            perror("Send failed");
            return 1;
        }
    }

    //The socket is closed using close() after the message loop ends

    close(sock);
    return 0;
}

// Thread to receive messages from the server  waits for incoming messages from the server.
void *receive_messages(void *sock_desc) {
    int sock = *(int *)sock_desc;
    char server_reply[BUFFER_SIZE];  
    int bytes_received;

    // Continuously receive messages from the server
    while ((bytes_received = recv(sock, server_reply, sizeof(server_reply), 0)) > 0) { ////The recv() function is used to read the server’s messages into server_reply. The message is then printed to the console.
        server_reply[bytes_received] = '\0';  // Null-terminate the message
        printf("\n%s\n", server_reply);
    }

    //If the connection is closed or an error occurs, the thread terminates.

    return NULL;
}

//It uses the pthread library for multi-threading to allow the user to send messages in one thread while receiving messages from the server in another.

 //gcc client.c -o client -lpthread
// ./server
// ./client     client 1
// ./client     client 2