
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int server_socket, client_socket;
    struct sockaddr_in server;
    char message[1024];

    // Create TCP socket
    server_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(server_socket < 0)
    {
        printf("Socket creation failed!\n");
        return 1;
    }

    // Configure server
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = INADDR_ANY;

    // Bind socket
    if(bind(server_socket, (struct sockaddr *)&server,
            sizeof(server)) < 0)
    {
        printf("Bind failed!\n");
        close(server_socket);
        return 1;
    }

    // Listen for client
    listen(server_socket, 5);

    printf("Server waiting for client...\n");

    // Accept client
    client_socket = accept(server_socket, NULL, NULL);

    if(client_socket < 0)
    {
        printf("Connection failed!\n");
        close(server_socket);
        return 1;
    }

    printf("Client connected!\n");

    // Two-way communication
    while(1)
    {
        memset(message, 0, sizeof(message));

        printf("\nClient: ");

        int bytes = recv(client_socket, message,
                         sizeof(message) - 1, 0);

        if(bytes <= 0)
            break;

        message[bytes] = '\0';

        printf("%s\n", message);

        if(strcmp(message, "exit") == 0)
            break;

        printf("Server: ");
        fgets(message, sizeof(message), stdin);

        message[strcspn(message, "\n")] = '\0';

        send(client_socket, message, strlen(message), 0);

        if(strcmp(message, "exit") == 0)
            break;
    }

    printf("\nChat ended.\n");

    close(client_socket);
    close(server_socket);

    return 0;
}

