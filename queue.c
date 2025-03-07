#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

typedef struct cellType cellType;

struct queue{
    cellType * in;
    cellType * out;
};

struct cellType{
    void *data;
    cellType *next;
};

queue_type * queue_create(){
    queue_type * queue = malloc(sizeof(queue_type));

    queue->in = queue->out = NULL;

    return queue;
}

void enqueue(queue_type * queue, void * data){
    cellType * cell = malloc(sizeof(cellType));

    cell->data = data;
    cell->next = NULL;
    if(!queue->out) queue->out = cell;
    if(queue->in) queue->in->next = cell;

    queue->in = cell;
}

void * dequeue(queue_type * queue){
    if(queue->out == NULL) return NULL;
    cellType * aux = queue->out;

    queue->out = aux->next;
    if(queue->out == NULL) queue->in = NULL;

    void * data = aux->data;
    free(aux);

    return data;
}

void queue_free(queue_type * queue){
    free(queue);
}

int empty(queue_type * queue){
    return !queue->out;
}