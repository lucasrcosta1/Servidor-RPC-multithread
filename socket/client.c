#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

int main(void){
    int socket_created;
    struct sockaddr_in server_addr;
    char server_message[2000], client_message[2000];
    int server_struct_length = sizeof(server_addr);
    
    // Clean buffers:
    memset(server_message, '\0', sizeof(server_message));
    memset(client_message, '\0', sizeof(client_message));
    
    // Create socket:
    socket_created = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    if(socket_created < 0){
        printf("Error while creating socket\n");
        return -1;
    }
    printf("Socket created successfully\n");
    
    // Set port and IP:
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(3333);
    server_addr.sin_addr.s_addr = inet_addr("192.168.99.139");
    
    // Get input from the user:
    printf("Enter message: ");
    scanf("%s",client_message);
    
    // Send the message to server:
    if (sendto (
        socket_created, 
        (const void *)client_message, 
        (size_t)strlen(client_message), 
        0,
        (const struct sockaddr*)&server_addr, 
        (socklen_t)server_struct_length
    ) < 0) {
        printf("Unable to send message\n");
        return -1;
    }
    
    // Receive the server's response:
    if (recvfrom (
        socket_created, 
        server_message, 
        sizeof(server_message), 
        0,
        (struct sockaddr*)&server_addr, 
        &server_struct_length
    ) < 0) {
        printf("Error while receiving server's msg\n");
        return -1;
    }
    
    printf("Server's response: %s\n", server_message);
    
    // Close the socket:
    close(socket_created);
    
    return 0;
}
