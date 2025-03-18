#ifndef DISK_H
#define DISK_H

typedef struct disk disk;

#include <stdio.h>
#include "BT.h"

/**
 * @brief Cria o disco de armazenamento da árvore
 * @param name Nome do arquivo binário a ser criado
 * @param order Ordem da árvore armazenada
 * @return Disco gerado
 */
disk * disk_create(char * name, int order);

/**
 * @brief Grava um nó no disco
 * @param d Disco em que será grava o nó
 * @param node Nó a ser gravado
 * @param new 0 para nova gravação e 1 para alteração de dados em nó já gravado
 */
long disk_write(disk * d, node_type * node, int new);

node_type * disk_read(disk * d, long bp);

void disk_free(disk * d);

#endif