#include <math.h>
#include <stdlib.h>
#include "disk.h"

#define DEBUG 0
#define UNDEFINED -1

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
    d -> node_size = sizeof(long) + 3 * sizeof(int) + 2 * (order - 1) * sizeof(int) + order * sizeof(long);

return d;
}

long disk_write(disk * d, node_type * node, int new) {

    // Os valores indefinidos servem para os preencher espaços do nó que ainda não foram preenchidos pelo cliente
    int nan = UNDEFINED;
    long lnan = UNDEFINED;

    // Os dados do nó são guardados um a um e a ordem é importante para a leitura
    long bp = node_get_bp(node);
    int size = node_get_size(node);
    int leaf = node_get_leaf(node);
    int children_quantity = node_get_children_quantity(node);
    int * keys = node_get_keys(node);
    int * values = node_get_values(node);
    long * cbps = node_get_children(node);

    // O parâmetro new (quando diferente de 0) indica que o cliente deseja sobrescrever um nó
    if(new) {
        fseek(d -> file, d -> bp, 0);
        fwrite(&d -> bp, sizeof(long), 1, d -> file);
    }
    else {
        fseek(d -> file, bp, 0);
        fwrite(&bp, sizeof(long), 1, d -> file);
    }
    
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
        if(i < children_quantity) fwrite(&cbps[i], sizeof(long), 1, d -> file);
        else fwrite(&lnan, sizeof(long), 1, d -> file);
    }

    // Se o cliente não declarou o nó como new, ele apenas está o sobrescrevendo e não quer um novo bp
    if(!new) return bp;

    // Caso contrário, retornamos o bp do momento em que iniciamos a gravação dos dados
    long old_bp = d -> bp;
    d -> bp += d -> node_size;

return old_bp;
}

node_type * disk_read(disk * d, long bp) {

    long bp;
    int size, leaf, children_quantity, keys[d -> order], values[d -> order], cbps[d -> order];

    fseek(d -> file, d -> bp, 0);
    fread(&bp, sizeof(long), 1, d -> file);
    fread(&size, sizeof(int), 1, d -> file);
    fread(&leaf, sizeof(int), 1, d -> file);
    fread(&children_quantity, sizeof(int), 1, d -> file);

    for(int i = 0; i < d -> order; i++) fread(&keys[i], sizeof(int), 1, d -> file);
    for(int i = 0; i < d -> order; i++) fread(&values[i], sizeof(int), 1, d -> file);
    for(int i = 0; i < d -> order; i++) fread(&cbps[i], sizeof(long), 1, d -> file);

    return node_read(bp, size, leaf, children_quantity, keys, values, cbps);
}

void disk_free(disk * d) {

    fclose(d -> file);
    free(d);

}