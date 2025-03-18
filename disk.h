#ifndef DISK_H
#define DISK_H

typedef struct disk disk;

#include <stdio.h>
#include "BT.h"

disk * disk_create(char * path, int order);

long disk_write(disk * d, node_type * node);

void * disk_read(disk * d, long bp);

// void disk_remove?

void disk_free(disk * d);

#endif