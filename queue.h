/**
 * 2o Trabalho de Técnicas de Busca e Ordenação (2024/02)
 * Professor: Luis Antonio de Souza Junior
 * Alunos: Conrado Antoniazi (2023100576), Eduardo Silva (2023100580) e Gabriel Sena (2023100257)
 */

#ifndef queue_h
#define queue_h

typedef struct queue queue_type;

queue_type * queue_create();

void enqueue(queue_type * queue, void * data);

void * dequeue(queue_type * queue);

void queue_free(queue_type * queue);

int empty(queue_type * queue);

#endif