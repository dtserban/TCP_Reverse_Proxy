// TCP_Reverse_Proxy_Server -- Daniel Serban
// Serverside code to accept connections from clientside code and outside clients and act as reverse proxy

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

// Struct for passing connected socket info
struct tunnel_socks
{
    int recv_from;
    int send_to;
};

// Sends traffic from connected user to server through reverse proxy (and vice versa)
void* send_through(void* bar)
{
    char buff[1536] = { 0 };
    int numbyte = 0;
    struct tunnel_socks* tunnel_dat = (struct tunnel_socks*)bar;

    while (1)
    {
        numbyte = recv(tunnel_dat->recv_from, buff, 1536, 0);

        if (numbyte > 0)
        {
            send(tunnel_dat->send_to, buff, numbyte, 0);
        }
    }
}

int main(int argc, char const* argv[])
{
    int client_sockfd = 0;
    int server_sockfd = 0;
    int client_conn = 0;
    int server_conn = 0;

    struct sockaddr_in server_dat;
    struct sockaddr_in client_dat;

    int dat_len = sizeof (struct sockaddr_in);

    struct tunnel_socks to_client;
    struct tunnel_socks to_server;

    pthread_t client_th, server_th;

    char* OKsig = "OK";

    // For reading user command
    char* foo = NULL;
    size_t num = 5;
    foo = malloc(num);

    // Fill sockaddr_in structs
    client_dat.sin_family = AF_INET;
    client_dat.sin_addr.s_addr = INADDR_ANY;
    client_dat.sin_port = htons(45321);

    server_dat.sin_family = AF_INET;
    server_dat.sin_addr.s_addr = INADDR_ANY;
    server_dat.sin_port = htons(54321);

    // Create and connect sockets...
    printf("Waiting for server to connect.\n");

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(server_sockfd, (struct sockaddr*)&server_dat, dat_len);
    listen(server_sockfd, 5);
    server_conn = accept(server_sockfd, NULL, NULL);

    printf("Server connected to socket.\n");

    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    bind(client_sockfd, (struct sockaddr*)&client_dat, dat_len);
    listen(client_sockfd, 5);
    client_conn = accept(client_sockfd, NULL, NULL);

    printf("Client connected to socket.\n");

    // Fill tunnel_socks structs
    to_client.recv_from = server_conn;
    to_client.send_to = client_conn;

    to_server.recv_from = client_conn;
    to_server.send_to = server_conn;

    // Send "OK" signal to complete tunnel connection in user's clientside code
    send(to_server.send_to, OKsig, 2, 0);

    printf("Signal 'OK' sent.\n");

    // Start client and server communication threads to read from them
    pthread_create(&server_th, NULL, send_through, (void*)&to_client);
    pthread_create(&client_th, NULL, send_through, (void*)&to_server);

    while (strcmp(foo, "exit\n"))
    {
        printf("Type exit to close the tunnel.\n");
        getline(&foo, &num, stdin);
    }

    // Pthread cancel code here...
    printf("Closing tunnel connections...\n");
    pthread_cancel(client_th);
    pthread_cancel(server_th);

    free(foo);
    return 0;
}
