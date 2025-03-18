#include <math.h>
#include <stdlib.h>
#include "disk.h"

#define DEBUG 0

struct disk {
    FILE * file;
    long bp;    // Binary pointer
    int order;
    int node_size;
};

disk * disk_create(char * name, int order) {

    disk * d = (disk *) calloc(1, sizeof(disk));
    d -> file = fopen(name, "wb+");
    d -> bp = 0;
    d -> order = order;
    d -> node_size = sizeof(long) + 2 * sizeof(int) + 2 * (order - 1) * sizeof(int) + order * sizeof(long);

return d;
}

long disk_write(disk * d, node_type * node) {

    int nan = -1;
    int size = node_get_size(node);
    int leaf = node_get_leaf(node);
    int children_quantity = node_get_children_quantity(node);
    int * keys = node_get_keys(node);
    int * values = node_get_values(node);
    long * cbps; // missing

    fseek(d -> file, d -> bp, 0);
    fwrite(&d -> bp, sizeof(long), 1, d -> file);
    fwrite(&size, sizeof(int), 1, d -> file);
    fwrite(&leaf, sizeof(int), 1, d -> file);
    fwrite(&children_quantity, sizeof(int), 1, d -> file);

    for(int i = 0; i < d -> order; i++) {
        if(i < size) fwrite(&keys[i], sizeof(int), 1, d -> file);
        else fwrite(&nan, sizeof(int), 1, d -> file);
    }
    for(int i = 0; i < d -> order; i++) {
        if(i < size) fwrite(&values[i], sizeof(int), 1, d -> file);
        else fwrite(&nan, sizeof(int), 1, d -> file);
    }
    for(int i = 0; i < d -> order; i++) {
        //fwrite(&values[i], sizeof(int), 1, d -> file);    MISSING 
    }

    long int bp = d -> bp;
    d -> bp += d -> node_size;

return bp;
}

void disk_change();

node_type * disk_read(disk * d, long int bp) {

    long bp;
    int size, leaf, children_quantity, keys[d -> order], values[d -> order], cbps[d -> order];

    fseek(d -> file, d -> bp, 0);
    fread(&bp, sizeof(long), 1, d -> file);
    fread(&size, sizeof(int), 1, d -> file);
    fread(&leaf, sizeof(int), 1, d -> file);
    fread(&children_quantity, sizeof(int), 1, d -> file);

    for(int i = 0; i < d -> order; i++) fread(&keys[i], sizeof(int), 1, d -> file);
    for(int i = 0; i < d -> order; i++) fread(&values[i], sizeof(int), 1, d -> file);
    // for(int i = 0; i < d -> order; i++) MISSING BPS

    node_type * node = node_read(bp, size, leaf, children_quantity, keys, values, cbps);

return node;
}

void disk_free(disk * d) {

    fclose(d -> file);
    free(d);

}