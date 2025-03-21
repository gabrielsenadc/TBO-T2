#ifndef BT_H
#define BT_H

#include <stdio.h>

typedef struct node node_type;

#include "disk.h"

typedef struct BT BT_type;

/**
 * @brief Cria um ponteiro para um nó a partir de dados pré-existentes
 * @param bp Ponteiro do nó no arquivo binário
 * @param size Quantidade inteira de dados salvos no nó até o momento
 * @param leaf Diz se o nó é folha
 * @param children_quantity Quantidade de filhos que o nó possui
 * @param keys Vetor de inteiros contendo as chaves do nó
 * @param values Vetor de inteiros contendo os valores de cada chave
 * @param cbps Vetor de inteiros longos contendo os ponteiros binários de cada filho do nó
 * @return Ponteiro para o nó lido - A MEMÓRIA DEVE SER LIBERADA
 */
node_type * node_read(long bp, int size, int leaf, int order, int * keys, int * values, long * cbps);

/**
 * @brief Retorna a quantidade de dados salvas no nó
 * @param node Nó que se deseja obter o dado
 * @return Quantidade de dados
 */
int node_get_size(node_type * node);

/**
 * @brief Retorna se o nó é folha
 * @param node Nó que se deseja obter o dado
 * @return 1 para nó folha, 0 caso contrário
 */
int node_get_leaf(node_type * node);

/**
 * @brief Retorna o ponteiro do nó no arquivo binário (disco)
 * @param node Nó que se deseja obter o dado
 * @return Ponteiro do binário
 */
long node_get_bp(node_type * node);

/**
 * @brief Retorna as chaves do nó
 * @param node Nó que se deseja obter o dado
 * @return Chaves do nó
 */
int * node_get_keys(node_type * node);

/**
 * @brief Retorna os valores do nó
 * @param node Nó que se deseja obter o dado
 * @return Valores do nó
 */
int * node_get_values(node_type * node);

/**
 * @brief Retorna os ponteiros dos filhos do nó
 * @param node Nó que se deseja obter o dado
 * @return Ponteiros dos filhos do nó
 */
long * node_get_children(node_type * node);

/**
 * @brief Cria uma ávore B de uma determinada ordem
 * @param order ordem da árvore a ser criada
 * @return Ponteiro para a árvore criada
 */
BT_type * BT_create(int order);

/**
 * @brief Insere um par chave - valor em uma árvore B
 * @param BT árvore B onde será inserido a chave - valor
 * @param key chave a ser inserida
 * @param value valor a ser inserido, associado à chave
 */
void BT_insert(BT_type * BT, int key, int value);

/// @brief Função de busca na árvore
/// @param root Raiz da árvore
/// @param key Chave a ser buscada
/// @return Retorna 1 para caso a chave esteja na árvore e 0 para o caso contrário
int BT_search(BT_type * BT, node_type * root, int key);

/**
 * @brief Retorna uma determinada chave de uma árvore B, 
 * se a chave passada não pertencer à árvore, nada muda
 * @param BT árvore B onde ocorrerá a remoção
 * @param key chave a ser removida
 */
void BT_remove(BT_type * BT, int key);

/**
 * @brief Imprime uma determinada árvore B em largura
 * @param BT árvore B que deve ser imprimida
 * @param output arquivo de impressão
 */
void BT_print(BT_type * BT, FILE * output);

/**
 * @brief Desaloca devidamente uma árvore B da memória
 * @param BT árvore B que deve ser liberada
 */
void BT_free(BT_type * BT);

/**
 * @brief Retorna a raiz de uma árvore B
 * @param BT árvore B que se deseja obter o dado
 * @return node que representa a raiz
 */
node_type * BT_get_root(BT_type * BT);

#endif