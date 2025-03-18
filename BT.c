#include <stdlib.h>
#include "BT.h"
#include "queue.h"

struct node {
    long pos_binary_file;     //node position in the binary file   
    int size;     //amount of keys in this node
    int leaf;     //flag if node is a leaf  
    int children_quantity;  //amount of children
    int * keys;     //array of keys
    int * values;     //array o values, each indexed by a key
    node_type ** children;     //array of children
};

node_type* node_create(int order, int is_leaf) {
    node_type* node = calloc(1, sizeof(node_type));

    if (node == NULL) printf("Erro ao criar um nó vazio\n");

    node->leaf = is_leaf;  // Aqui definimos a flag leaf explicitamente

    node->keys = calloc(order, sizeof(int));
    node->values = calloc(order, sizeof(int));
    node->children = calloc(order + 1, sizeof(node_type* ));

    return node;
}

node_type * node_read(long bp, int size, int leaf, int children_quantity, int * keys, int * values, long * cbp) {

    node_type * node = (node_type *) calloc(1, sizeof(node_type));
    node->keys = calloc(size, sizeof(int));
    node->values = calloc(size, sizeof(int));

    node -> pos_binary_file = bp;
    node -> size = size;
    node -> leaf = leaf;
    node -> children_quantity = children_quantity;
    for(int i = 0; i < size; i++) node -> keys[i] = keys[i];
    for(int i = 0; i < size; i++) node -> values[i] = values[i];
    // Missing children bps

return node; 
}

int node_get_size(node_type * node){
    if(node == NULL) return 0;
    return node->size;
}

int node_get_leaf(node_type * node) {
    return node -> leaf;
}

long int node_get_bp(node_type * node) {
    return node -> pos_binary_file;
}

int * node_get_keys(node_type * node) {
    return node -> keys;
}

int * node_get_values(node_type * node) {
    return node -> values;
}

int node_get_children_quantity(node_type * node) {
    return node -> children_quantity;
}

void node_free(node_type * node) {
    if (node == NULL) return;

    if (!node->leaf) for (int i = 0; i <= node->size; i++) node_free(node->children[i]);
    free(node->children);
    free(node->keys);
    free(node->values);

    free(node);
}

node_type * node_get_righter(node_type * node){
    if(node->leaf) return node;
    return node_get_righter(node->children[node->size]);
}

node_type * node_get_lefter(node_type * node){
    if(node->leaf) return node;
    return node_get_lefter(node->children[0]);
}

node_type * node_get_sibling(int i, node_type * parent, int right){
    if(parent == NULL) return NULL;

    if(right) i++;
    else i--;

    if(i > parent->size) return NULL;
    if(i < 0) return NULL;

    return parent->children[i];
}

void node_concat(node_type * dest, node_type * src){
    int index = dest->size;
    for(int i = 0; i < src->size; i++){
        dest->keys[index + i] = src->keys[i];
        dest->values[index + i] = src->values[i];
        dest->children[index + i] = src->children[i];
    }
    dest->size += src->size;
    dest->children[dest->size] = src->children[src->size];

    for(int i = 0; i <= src->size; i++){
        src->children[i] = NULL;
    }

}

void node_print(node_type * node){
    if(node == NULL) return;
    printf("[");

    for(int i = 0; i < node_get_size(node); i++) printf("key: %d, ", node->keys[i]);

    printf("]");
}

struct BT {
    int order;     //BT order
    node_type * root;     //poiter to the root node
    int size;     //total number of nodes in this BT
};


BT_type * BT_create(int order){
    BT_type * BT = malloc(sizeof(BT_type));

    BT->order = order;
    BT->root = NULL;
    BT->size = 0;

    return BT;
}

int BT_get_min(BT_type * BT){
    int order = BT->order;
    if(BT->order % 2 != 0) order++;
    return (order / 2) - 1;
}


static void BT_split(node_type* parent, int index, node_type* node, int order) {
    int mediana_index = (order - 1) / 2;

    node_type* sibling = node_create(order, node->leaf);
    sibling->size = node->size - mediana_index - 1;
    node->size = mediana_index;

    // Transfere chaves após a mediana para o irmão
    for (int i = 0; i < sibling->size; i++) {
        sibling->keys[i] = node->keys[i + mediana_index + 1];
        sibling->values[i] = node->values[i + mediana_index + 1];
    }

    if (!node->leaf) {
        for (int j = 0; j <= sibling->size; j++) {
            sibling->children[j] = node->children[j + mediana_index + 1];
        }
    }

    // Insere a mediana no pai
    for (int i = parent->size - 1; i >= index; i--) {
        parent->keys[i + 1] = parent->keys[i];
        parent->values[i + 1] = parent->values[i];
    }
    // Desloca filhos apenas a partir de index + 1
    for (int j = parent->size; j >= index + 1; j--) {
        parent->children[j + 1] = parent->children[j];
    }
    parent->keys[index] = node->keys[mediana_index];
    parent->values[index] = node->values[mediana_index];
    parent->children[index] = node;
    parent->children[index+1] = sibling;

    // Ajusta o tamanho do nó original e do pai
    parent->size++;
}


static void BT_insert_nonfull_2(node_type* node, int order, int key, int value) {
    int index = node->size - 1;

    // CASO 1: Nó é folha
    if (node->leaf) {
        // Desloca chaves e valores maiores para a direita
        while (index >= 0 && key < node->keys[index]) {
            node->keys[index + 1] = node->keys[index];
            node->values[index + 1] = node->values[index];
            index--;
        }
        // Insere a nova chave e valor
        node->keys[index + 1] = key;
        node->values[index + 1] = value;
        node->size++;
    }
    // CASO 2: Nó é interno
    else {
        // Encontra a posição do filho para descer
        while (index >= 0 && key < node->keys[index]) {
            index--;
        }
        index++;

        BT_insert_nonfull_2(node->children[index], order, key, value);
        if(node->children[index]->size == order) BT_split(node, index, node->children[index], order);
    }
}


static void BT_insert_nonfull(node_type* node, int order, int key, int value) {
    int index = node->size - 1;

    // CASO 1: Nó é folha
    if (node->leaf) {
        // Desloca chaves e valores maiores para a direita
        while (index >= 0 && key < node->keys[index]) {
            node->keys[index + 1] = node->keys[index];
            node->values[index + 1] = node->values[index];
            index--;
        }
        // Insere a nova chave e valor
        node->keys[index + 1] = key;
        node->values[index + 1] = value;
        node->size++;
    }
    // CASO 2: Nó é interno
    else {
        // Encontra a posição do filho para descer
        while (index >= 0 && key < node->keys[index]) {
            index--;
        }
        index++;

        // Split caso filho esteja cheio
        if (node->children[index] && node->children[index]->size == order - 1) {
            BT_split(node, index, node->children[index], order);

            // Se a nova chave for maior que a chave promovida, vamos para o próximo filho
            if (key > node->keys[index]) {
                index++;
            }
        }
        // Desce recursivamente
        BT_insert_nonfull(node->children[index], order, key, value);
    }
}


void BT_insert(BT_type * BT, int key, int value) {
    if (BT == NULL) return;   // Árvore inválida para inserção

    if (BT->root == NULL) BT->root = node_create(BT->order, 1);

    node_type* root = BT->root;

    BT_insert_nonfull_2(root, BT->order, key, value);
    if (root->size == BT->order) {
        // Cria novo nó que será a nova raiz
        node_type* new_root = node_create(BT->order, 0);
        BT->root = new_root;
        // O antigo root vira filho[0] da nova raiz
        new_root->children[0] = root;
        BT_split(new_root, 0, root, BT->order);
    }
}


int BT_search(node_type * root, int key) {
    if(root == NULL) return 0;   //Nó invalido para busca

    int index = 0, node_size = node_get_size(root);
    while(index < node_size && key > root->keys[index]) index++;

    if(index < node_size && key == root->keys[index]) return 1;
    else if(root->leaf) return 0;  //Não encontrou a chave
    else return BT_search(root->children[index], key);    //Continua a busca nos outros nós
}


node_type * remove_key(BT_type * BT, node_type * node, int key);

node_type * fix_caso3(BT_type * BT, node_type * parent, node_type * node, int i_parent){
    if(node->size >= BT_get_min(BT)) return parent;

    node_type * left_sibling = node_get_sibling(i_parent, parent, 0);
    node_type * right_sibling = node_get_sibling(i_parent, parent, 1);

    if(node_get_size(left_sibling) > BT_get_min(BT)){
        i_parent--;

        node->size++;
        node->children[node->size] = node->children[node->size - 1];
        for(int i = node->size - 1; i > 0; i--){
            node->keys[i] = node->keys[i - 1];
            node->values[i] = node->values[i - 1];
            node->children[i] = node->children[i - 1];
        }
        
        int i_sibling = node_get_size(left_sibling) - 1;
        
        node->keys[0] = parent->keys[i_parent];
        node->values[0] = parent->values[i_parent];

        parent->keys[i_parent] = left_sibling->keys[i_sibling];
        parent->values[i_parent] = left_sibling->values[i_sibling];
        
        node->children[0] = left_sibling->children[left_sibling->size];

        left_sibling->size--;


    } else if(node_get_size(right_sibling) > BT_get_min(BT)){

        node->size++;

        int i_sibling = 0;
        
        node->keys[node->size - 1] = parent->keys[i_parent];
        node->values[node->size - 1] = parent->values[i_parent];

        parent->keys[i_parent] = right_sibling->keys[i_sibling];
        parent->values[i_parent] = right_sibling->values[i_sibling];

        node->children[node->size] = right_sibling->children[0];

        for(int i = 0; i < right_sibling->size - 1; i++){
            right_sibling->keys[i] = right_sibling->keys[i + 1];
            right_sibling->values[i] = right_sibling->values[i + 1];
            right_sibling->children[i] = right_sibling->children[i + 1];
        }
        right_sibling->children[right_sibling->size - 1] = right_sibling->children[right_sibling->size];

        right_sibling->size--;

    } else {
        if(right_sibling == NULL) i_parent--;

        int key_parent = parent->keys[i_parent];

        if(right_sibling){
            node->size++;
            node->children[node->size] = NULL;

            node->keys[node->size - 1] = parent->keys[i_parent];
            node->values[node->size - 1] = parent->values[i_parent];
            node_concat(node, right_sibling);   
            node_free(right_sibling);
        } else {
            left_sibling->size++;
            left_sibling->children[left_sibling->size] = NULL;

            left_sibling->keys[left_sibling->size - 1] = parent->keys[i_parent];
            left_sibling->values[left_sibling->size - 1] = parent->values[i_parent];

            node_concat(left_sibling, node);   
            node_free(node);
        }

        for(int i = 0; i < parent->size; i++){
            if(parent->keys[i] > key_parent){
                parent->keys[i - 1] = parent->keys[i];
                parent->values[i - 1] = parent->values[i];
                parent->children[i] = parent->children[i + 1];
            } 
        }

        parent->size--;

        if(parent->size == 0 && BT->root == parent) {
            node_type * child = parent->children[0];
            for(int i = 0; i < BT->order; i++) parent->children[i] = NULL;
            node_free(parent);
            return child;
        }
    }

    return parent;
}

node_type *  remove_key_caso1(BT_type * BT, node_type * node, int key){
    for(int i = 0; i < node->size; i++){
        if(node->keys[i] > key){
            node->keys[i - 1] = node->keys[i];
            node->values[i - 1] = node->values[i];
        } 
    }

    node->size--;

    return node;
}

node_type * remove_key_caso2(BT_type * BT, node_type * node, int key){
    int i = 0;
    for(; i < node->size; i++) if(key == node->keys[i]) break;

    if(node_get_size(node->children[i + 1]) <= BT_get_min(BT)){
        node_type * righter = node_get_righter(node->children[i]);
        int j = node_get_size(righter) - 1;

        node->keys[i] = righter->keys[j];
        node->values[i] = righter->values[j];

        node->children[i] = remove_key(BT, node->children[i], righter->keys[j]);
        return fix_caso3(BT, node, node->children[i], i);

    } else {
        node_type * lefter = node_get_lefter(node->children[i + 1]);
        int j = 0;

        node->keys[i] = lefter->keys[j];
        node->values[i] = lefter->values[j];

        node->children[i + 1] = remove_key(BT, node->children[i + 1], lefter->keys[j]);
        return fix_caso3(BT, node, node->children[i + 1], i + 1);
    }

}

node_type * remove_key(BT_type * BT, node_type * node, int key){
    int i = 0;
    for(; i < node->size; i++) if(key <= node->keys[i]) break;

    if(i < node->size && key == node->keys[i]){
        if(node->leaf) node = remove_key_caso1(BT, node, key);
        else node = remove_key_caso2(BT, node, key);

        return node;
    }

    if(node->leaf) printf("NAO EXISTE");
    else node->children[i] = remove_key(BT, node->children[i], key);

    return fix_caso3(BT, node, node->children[i], i);
}

void BT_remove(BT_type * BT, int key){
    BT->root = remove_key(BT, BT->root, key);
}

void BT_print(BT_type * BT){
    queue_type * queue = queue_create();

    printf("--- ARVORE B\n");
    enqueue(queue, BT->root);
    int i = 1;

    node_type * node;
    while(!empty(queue)){
        int qtt_filhos = 0;
        for(int j = 0; j < i; j++){
            node = dequeue(queue);
            node_print(node);
            if(node_get_size(node) > 0 && !node->leaf) {
                for(int k = 0; k <= node_get_size(node); k++){
                    node_type * add = node->children[k];
                    enqueue(queue, add);
                } 
                qtt_filhos += node_get_size(node) + 1;
            }
            if(j != i - 1) printf(" ");
        }
        i = qtt_filhos;
        printf("\n");
    }

    queue_free(queue);
}

void BT_free(BT_type * BT){
    node_free(BT->root);
    free(BT);
}

node_type * BT_get_root(BT_type * BT) {
    if(BT != NULL) return BT->root;
    return NULL;
}