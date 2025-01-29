#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h> //Provides access to the POSIX operating system API, such as close to close sockets.
#include <arpa/inet.h> //Defines structures and functions related to internet addresses and protocols, such as sockaddr_in, inet_addr, etc.
#include <pthread.h> //Provides support for multi-threading using the POSIX threads library.

#define PORT 8080
#define BUFFER_SIZE 1024
#define MAX_CLIENTS 4


//Global Variables:
int client_sockets[MAX_CLIENTS] = {0};  // Array to store client sockets file
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;  // Mutex for thread safety
 
void *handle_client(void *client_socket);
int main() {
    int server_sock, client_sock, c;
    struct sockaddr_in server, client;
    pthread_t thread_id;

    // Create socket
    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    //Creates a socket using socket() with the AF_INET address family (IPv4) and SOCK_STREAM type (TCP).
    if (server_sock == -1) {
        printf("Could not create socket\n");;
        return 1;
    }
    printf("Socket created.\n");

    // Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(PORT);

    // Bind
    if (bind(server_sock, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind failed. Error");
        return 1;
    }
    printf("Bind done.\n");

    // Listen for up to MAX_CLIENTS connections
    listen(server_sock, MAX_CLIENTS); //Listens for incoming client connections using listen().
    printf("Waiting for connections...\n");

    c = sizeof(struct sockaddr_in);

    // Accept incoming connections
    while ((client_sock = accept(server_sock, (struct sockaddr *)&client, (socklen_t *)&c)) > 0) {
        printf("Connection accepted.\n");

        // Find an empty slot for the new client
        pthread_mutex_lock(&lock);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] == 0) {
                client_sockets[i] = client_sock;

                // Create a thread for each client
                // A new thread is created using pthread_create() for each client. The thread handles communication with the client in the handle_client() function.
                if (pthread_create(&thread_id, NULL, handle_client, (void *)&client_sockets[i]) < 0) {
                    perror("Could not create thread");
                    return 1;
                }
                pthread_detach(thread_id);  // Detach the thread to free resources when it finishes
                break;
            }
        }
        pthread_mutex_unlock(&lock);
    }  //The while loop continues accepting connections as long as accept() returns a valid client socket.

    if (client_sock < 0) {
        perror("Accept failed");
        return 1;
    }

    return 0;
}

// Handle communication with a client
void *handle_client(void *client_socket) {
    //This function runs in a separate thread for each client. It receives messages from the connected client using recv().
    int sock = *(int *)client_socket;
    char buffer[BUFFER_SIZE];
    int bytes_read;

    //After receiving a message, the server broadcasts it to all other connected clients. This is done by iterating over the client_sockets array and sending the message to each client socket except the sender using send().

    // Keep receiving messages from the client
    while ((bytes_read = recv(sock, buffer, sizeof(buffer), 0)) > 0) {
        buffer[bytes_read] = '\0';  // Null-terminate the message
        printf("Received message: %s\n", buffer);  // Log message on server

        // Broadcast the message to other clients
        pthread_mutex_lock(&lock);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (client_sockets[i] != 0 && client_sockets[i] != sock) {
                if (send(client_sockets[i], buffer, bytes_read, 0) < 0) {
                    perror("Send failed");
                }
            }
        }
        pthread_mutex_unlock(&lock);
    }

    // If client disconnects

    //If the recv() call returns 0, it means the client has disconnected. In this case, the socket is closed using close()

    close(sock);
    pthread_mutex_lock(&lock);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (client_sockets[i] == sock) {
            client_sockets[i] = 0;  // Mark the slot as available
        }
    }
    pthread_mutex_unlock(&lock);

    return NULL;
}





// gcc server.c -o server -lpthread
// gcc client.c -o client
// 

// ./server
// ./client     client 1
// ./client     client 2
