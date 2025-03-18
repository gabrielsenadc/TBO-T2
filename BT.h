#ifndef BT_H
#define BT_H

#include <stdio.h>

typedef struct node node_type;
typedef struct BT BT_type;

node_type * node_read(long bp, int size, int leaf, int children_quantity, int * keys, int * values, long * cbp);

int node_get_size(node_type * node);

int node_get_leaf(node_type * node);

long int node_get_bp(node_type * node);

int * node_get_keys(node_type * node);

int * node_get_values(node_type * node);

int node_get_children_quantity(node_type * node);

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