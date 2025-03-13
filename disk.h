#ifndef DISK_H
#define DISK_H

#include <stdio.h>

typedef struct disk disk;

disk * disk_create(char * path, int order, int item_size);

int disk_is_full(disk * d);

int disk_get_levels(disk * d);

void disk_write(disk * d, void * data, long offset);

void * disk_read(disk * d, long offset);

void disk_free(disk * d);

#endif