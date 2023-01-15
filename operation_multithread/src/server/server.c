#include "server.h"

Server server;

sockaddr_in get_addr(u_int port)
{
    struct sockaddr_in address;

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = htonl(INADDR_ANY);
    address.sin_port = htons(port);

    return address;
}

void init_connection_loop()
{
    int * client;
    sockaddr_in client_address;
    socklen_t addr_size = (socklen_t) sizeof(sockaddr_in);

    while (true) {

        client = (int *) malloc(sizeof(int));

        (* client) = accept(server.socket, (struct sockaddr *) &client_address, &addr_size);

        if ((* client) == -1) {
            perror("accept failure");
        }

        sem_wait(&server.wait);

        enqueue(client);

        sem_post(&server.work_avaliable);
    }

    shutdown(server.socket, SHUT_RDWR);
}

void listen_connections()
{
    int optval = 1;

    setsockopt(server.socket, SOL_SOCKET, SO_REUSEADDR, (const void *)&optval, sizeof(int));
    bind(server.socket, (struct sockaddr *) &server.address, sizeof(struct sockaddr));
    listen(server.socket, server.backlog);

    init_connection_loop();
}

void server_config()
{
    server.port = DEFAULT_SERVER_PORT;
    server.backlog = DEFAULT_SERVER_BACKLOG;
    server.address = get_addr(DEFAULT_SERVER_PORT);
    server.socket = socket(AF_INET, SOCK_STREAM, 0);
}

void server_init()
{
    sem_init(&server.work_avaliable, PSHARED, RED);
    sem_init(&server.wait, PSHARED, GREEN);

    server.set_config = server_config;
    server.listen = listen_connections;
}