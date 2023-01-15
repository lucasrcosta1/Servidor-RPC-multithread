#ifndef QUEUE_H_INCLUDED
#define QUEUE_H_INCLUDED

#include <stdlib.h>

#include "queue.h"

typedef struct queue_node {
    struct queue_node * next;
    int * sockfd;
} queue_node;

extern queue_node * first;
extern queue_node * last;

void enqueue(int * sockfd);

int * dequeue();

#endif // QUEUE_H_INCLUDED
