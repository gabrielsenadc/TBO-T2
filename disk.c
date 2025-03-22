/**
 * 2o Trabalho de Técnicas de Busca e Ordenação (2024/02)
 * Professor: Luis Antonio de Souza Junior
 * Alunos: Conrado Antoniazi (2023100576), Eduardo Silva (2023100580) e Gabriel Sena (2023100257)
 */

#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "disk.h"
#include "colors.h"

#define DEBUG 0
#define UNDEFINED -1

struct disk {
    char * file_name;
    FILE * file;
    long bp;    // Binary pointer
    int order;
    int node_size;
};

disk * disk_create(char * name, int order) {

    disk * d = (disk *) calloc(1, sizeof(disk));
    d -> file_name = strdup(name);
    d -> file = fopen(name, "wb+");
    d -> bp = 0;
    d -> order = order;
    d -> node_size = sizeof(long) + 2 * sizeof(int) + 2 * (order - 1) * sizeof(int) + order * sizeof(long);

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
    int * keys = node_get_keys(node);
    int * values = node_get_values(node);
    long * cbps = node_get_children(node);

    // O parâmetro new (quando igual a 0) indica que o cliente deseja sobrescrever um nó
    if(!new) {
        if(bp == -1) {
            printf("%sVocê tentou sobrescrever um nó que nunca foi escrito\n%s", RED, RESET);
            exit(1);
        }
        fseek(d -> file, bp, 0);
        fwrite(&bp, sizeof(long), 1, d -> file);
    }
    else {
        fseek(d -> file, d -> bp, 0);
        fwrite(&d -> bp, sizeof(long), 1, d -> file);
    }
    
    fwrite(&size, sizeof(int), 1, d -> file);
    fwrite(&leaf, sizeof(int), 1, d -> file);

    for(int i = 0; i < d -> order - 1; i++) {
        if(i < size) fwrite(&keys[i], sizeof(int), 1, d -> file);
        else fwrite(&nan, sizeof(int), 1, d -> file);
    }
    for(int i = 0; i < d -> order - 1; i++) {
        if(i < size) fwrite(&values[i], sizeof(int), 1, d -> file);
        else fwrite(&nan, sizeof(int), 1, d -> file);
    }
    for(int i = 0; i < d -> order; i++) {
        if(i < size + 1) fwrite(&cbps[i], sizeof(long), 1, d -> file);
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

    int size, leaf, keys[d -> order], values[d -> order];
    long cbps[d -> order];

    fseek(d -> file, bp, 0);
    fread(&bp, sizeof(long), 1, d -> file);
    fread(&size, sizeof(int), 1, d -> file);
    fread(&leaf, sizeof(int), 1, d -> file);

    for(int i = 0; i < d -> order - 1; i++) fread(&keys[i], sizeof(int), 1, d -> file);
    for(int i = 0; i < d -> order - 1; i++) fread(&values[i], sizeof(int), 1, d -> file);
    for(int i = 0; i < d -> order; i++) fread(&cbps[i], sizeof(long), 1, d -> file);

    return node_read(bp, size, leaf, d -> order, keys, values, cbps);
}

void disk_free(disk * d) {

    remove(d -> file_name);
    free(d -> file_name);
    fclose(d -> file);
    free(d);

}