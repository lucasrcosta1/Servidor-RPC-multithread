#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#include "server.h"
#include "svc.h"

#define POOL_NUM 10

int main(int argc, char **argv)
{
    u_int i;
	pthread_t pools[POOL_NUM];

    server_init();

    for (i = 0; i < POOL_NUM; i++) {
        pthread_create(&(pools[i]), NULL, svc_thread_handler, &server);
    }

    for (i = 0; i < POOL_NUM; i++) {
        pthread_join(pools[i], NULL);
    }

    server.set_config();
    server.listen();
}
