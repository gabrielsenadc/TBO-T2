#include <math.h>
#include "disk.h"

struct disk {
    FILE * file;
    int order;
    int item_size;
    // As variáveis abaixo são realmente úteis?
    int items_max_levels;
    int items_quantity;
};

static int get_max(disk * d) {
    
    int max = 0;

    for(int i = 0; i <= d -> items_max_levels; i++) {
        max += pow(d -> order, i);
    }

    return max;
}

static int get_quantity(disk * d) {
    return d -> items_quantity;
}

disk * disk_create(char * name, int order, int item_size) {

    disk * d = (disk *) calloc(1, sizeof(disk));
    d -> file = fopen(name, "wb+");
    d -> order = order;
    d -> items_max_levels = 0;
    d -> items_quantity = 0;
    d -> item_size = item_size;

return d;
}

int disk_is_full(disk * d) {
    return get_quantity(d) >= get_max(d);  
}

int disk_get_levels(disk * d) {
    return d -> items_max_levels;
}

void disk_write(disk * d, void * data, long offset) {

    fseek(d -> file, offset, 0);

    if(disk_is_full(d)) {
        d -> items_max_levels++;
    }

    fwrite(data, d -> item_size, 1, d -> file);
    d -> items_quantity;

}

void * disk_read(disk * d, long offset) {

    fseek(d -> file, offset, 0);
    void * data = (void *) calloc(1, d -> item_size);
    fread(data, d -> item_size, 1, d -> file);

return data;
}

void disk_free(disk * d) {

    fclose(d -> file);
    free(d);

}