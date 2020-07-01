// TCP_Reverse_Proxy_Client -- Daniel Serban
// Clientside code to connect out to the serverside code and establish a tunnel
// into the private network for outside clients using the reverse proxy

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
    //int client_conn = 0;
    //int server_conn = 0;

    struct sockaddr_in server_dat;
    struct sockaddr_in client_dat;

    int dat_len = sizeof (struct sockaddr_in);

    struct tunnel_socks to_client;
    struct tunnel_socks to_server;

    pthread_t client_th, server_th;

    char SockSig[4] = { 0 };

    // For reading user command
    char* foo = NULL;
    size_t num = 5;
    foo = malloc(num);

    // Fill sockaddr_in structs
    client_dat.sin_family = AF_INET;
    inet_aton(argv[1], &client_dat.sin_addr);
    client_dat.sin_port = htons(54321);

    server_dat.sin_family = AF_INET;
    inet_aton("127.0.0.1", &server_dat.sin_addr);
    server_dat.sin_port = htons(56789);

    // Create and connect sockets...
    printf("Waiting to connect to public server.\n");

    client_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    connect(client_sockfd, (struct sockaddr*)&client_dat, (socklen_t)dat_len);

    printf("Connected to public server.\n");

    // Keeps checking until signal "OK" received
    while (strcmp(SockSig, "OK"))
    {
        recv(client_sockfd, SockSig, 2, 0);
    }

    server_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    connect(server_sockfd, (struct sockaddr*)&server_dat, (socklen_t)dat_len);

    printf("Connected to private server.\n");

    // Fill tunnel_socks structs
    to_client.recv_from = server_sockfd;
    to_client.send_to = client_sockfd;

    to_server.recv_from = client_sockfd;
    to_server.send_to = server_sockfd;

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
