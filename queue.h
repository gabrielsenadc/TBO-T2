#ifndef queue_h
#define queue_h

typedef struct queue queue_type;

queue_type * queue_create();

void enqueue(queue_type * queue, void * data);

void * dequeue(queue_type * queue);

void queue_free(queue_type * queue);

int empty(queue_type * queue);

#endif