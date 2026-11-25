
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int client_socket;
    struct sockaddr_in server;
    char message[1024];

    // Create TCP socket
    client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(client_socket < 0)
    {
        printf("Socket creation failed!\n");
        return 1;
    }

    // Configure server address
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = inet_addr("127.0.0.1");

    // Connect to server
    if(connect(client_socket, (struct sockaddr *)&server,
               sizeof(server)) < 0)
    {
        printf("Connection failed!\n");
        close(client_socket);
        return 1;
    }

    printf("Connected to server!\n");
    printf("Type 'exit' to stop chat.\n");

    // Two-way communication
    while(1)
    {
        printf("\nClient: ");

        fgets(message, sizeof(message), stdin);

        message[strcspn(message, "\n")] = '\0';

        send(client_socket, message, strlen(message), 0);

        if(strcmp(message, "exit") == 0)
            break;

        memset(message, 0, sizeof(message));

        int bytes = recv(client_socket, message,
                         sizeof(message) - 1, 0);

        if(bytes <= 0)
            break;

        message[bytes] = '\0';

        printf("Server: %s\n", message);

        if(strcmp(message, "exit") == 0)
            break;
    }

    printf("\nChat ended.\n");

    close(client_socket);

    return 0;
}
 

