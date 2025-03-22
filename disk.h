/**
 * 2o Trabalho de Técnicas de Busca e Ordenação (2024/02)
 * Professor: Luis Antonio de Souza Junior
 * Alunos: Conrado Antoniazi (2023100576), Eduardo Silva (2023100580) e Gabriel Sena (2023100257)
 */

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
 * @param new Diferente de 0 para nova gravação e 0 para alteração de dados em nó já gravado
 * @return Ponteiro do arquivo binário para o nó gravado
 */
long disk_write(disk * d, node_type * node, int new);

/**
 * @brief Lê um nó do disco
 * @param d Disco a ser lido
 * @param bp Ponteiro do arquivo binário (localização do nó)
 * @return Ponteiro para o nó lido - A MEMÓRIA DEVE SER LIBERADA
 */
node_type * disk_read(disk * d, long bp);

/**
 * @brief Libera a memória de um disco
 * @param d Disco a a ser liberado
 */
void disk_free(disk * d);

#endif