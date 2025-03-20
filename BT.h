#ifndef BT_H
#define BT_H

#include <stdio.h>

typedef struct node node_type;
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
node_type * node_read(long bp, int size, int leaf, int children_quantity, int * keys, int * values, long * cbps);

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
 * @brief Retorna a quantidade de filhos do nó
 * @param node Nó que se deseja obter o dado
 * @return Quantidade de filhos do nó
 */
int node_get_children_quantity(node_type * node);

long * node_get_children(node_type * node);

BT_type * BT_create(int order);

void BT_insert(BT_type * BT, int key, int value);

/// @brief Função de busca na árvore
/// @param root Raiz da árvore
/// @param key Chave a ser buscada
/// @return Retorna 1 para caso a chave esteja na árvore e 0 para o caso contrário
int BT_search(node_type * root, int key);

void BT_remove(BT_type * BT, int key);

void BT_print(BT_type * BT);

void BT_free(BT_type * BT);

node_type * BT_get_root(BT_type * BT);

#endif