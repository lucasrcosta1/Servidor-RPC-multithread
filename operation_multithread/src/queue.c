#include "queue.h"

queue_node * first = NULL;
queue_node * last = NULL;

void enqueue(int * sockfd)
{
    queue_node * new_node = malloc(sizeof(queue_node));

    new_node->sockfd = sockfd;
    new_node->next = NULL;

    if (last == NULL) {
        first = new_node;
    } else {
        last->next = new_node;
    }

    last = new_node;
}

int * dequeue()
{
    if (first == NULL) {
        return NULL;
    }

    int * result = first->sockfd;
    queue_node * temp = first;

    first = first->next;

    if (first == NULL) {
        last = NULL;
    }

    free(temp);

    return result;
}