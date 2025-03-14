#ifndef DISK_H
#define DISK_H

#include <stdio.h>

typedef struct disk disk;

disk * disk_create(char * path, int order, int data_size);

long int disk_write(disk * d, void * data);

void * disk_read(disk * d, long int bp);

// void disk_remove?

int disk_get_quantity(disk * d);

void disk_free(disk * d);

#endif