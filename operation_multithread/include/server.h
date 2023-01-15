#ifndef SERVER_H_INCLUDED
#define SERVER_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <errno.h>

#include "queue.h"

#define THREAD_SHARING 0
#define PSHARED THREAD_SHARING

#define GREEN 1
#define RED 0

#define DEFAULT_SERVER_PORT 3000
#define DEFAULT_SERVER_BACKLOG 5

typedef struct sockaddr_in sockaddr_in;

typedef struct server {
    int socket;
    int port;
    int backlog;
    sockaddr_in address;
    sem_t wait;
    sem_t work_avaliable;
    void (*listen)();
    void (*set_config)();
} Server;

extern Server server;

void server_init();

#endif