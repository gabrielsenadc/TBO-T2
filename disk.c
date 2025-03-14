#include <math.h>
#include <stdlib.h>
#include "disk.h"

#define DEBUG 0

struct disk {
    FILE * file;
    long int bp;    // Binary pointer
    int data_size;
    int data_quantity;
    void * (*data_reader)(FILE *);
};

disk * disk_create(char * name, int order, int data_size) {

    disk * d = (disk *) calloc(1, sizeof(disk));
    d -> file = fopen(name, "wb+");
    d -> data_size = data_size;
    d -> data_quantity = 0;
    d -> bp = 0;

return d;
}

long int disk_write(disk * d, void * data) {

    fseek(d -> file, d -> bp, 0);

    fwrite(data, d -> data_size, 1, d -> file);
    d -> data_quantity++;
    long int bp = d -> bp;
    d -> bp += d -> data_size;

return bp;
}

void * disk_read(disk * d, long int bp) {

    fseek(d -> file, bp, 0);
    void * data = (void *) calloc(1, d -> data_size);
    fread(data, d -> data_size, 1, d -> file);

return data;
}

int disk_get_quantity(disk * d) {
    return d -> data_quantity;
}

void disk_free(disk * d) {

    fclose(d -> file);
    free(d);

}