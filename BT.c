#include <stdlib.h>
#include "BT.h"
#include "queue.h"

struct node {
    long pos_binary_file;     //node position in the binary file   
    int size;     //amount of keys in this node
    int leaf;     //flag if node is a leaf  
    int * keys;     //array of keys
    int * values;     //array o values, each indexed by a key
    long * children;     //array of children
};

node_type* node_create(int order, int is_leaf) {
    node_type* node = calloc(1, sizeof(node_type));

    if (node == NULL) printf("Erro ao criar um nó vazio\n");

    node->pos_binary_file = -1;
    node->leaf = is_leaf;  // Aqui definimos a flag leaf explicitamente

    node->keys = calloc(order, sizeof(int));
    node->values = calloc(order, sizeof(int));
    node->children = calloc(order + 1, sizeof(node_type* ));

    return node;
}

node_type * node_read(long bp, int size, int leaf, int order, int * keys, int * values, long * cbps) {

    node_type * node = (node_type *) calloc(1, sizeof(node_type));
    node->keys = calloc(order, sizeof(int));
    node->values = calloc(order, sizeof(int));
    node -> children = calloc(order + 1, sizeof(long));

    node -> pos_binary_file = bp;
    node -> size = size;
    node -> leaf = leaf;
    for(int i = 0; i < size; i++) node -> keys[i] = keys[i];
    for(int i = 0; i < size; i++) node -> values[i] = values[i];
    for(int i = 0; i < size + 1; i++) node -> children[i] = cbps[i];
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

long node_get_bp(node_type * node) {
    return node -> pos_binary_file;
}

void node_set_bp(node_type * node, long bp) {
    node -> pos_binary_file = bp;
}

int * node_get_keys(node_type * node) {
    return node -> keys;
}

int * node_get_values(node_type * node) {
    return node -> values;
}

long * node_get_children(node_type * node) {
    return node -> children;
}

void node_free(node_type * node) {
    if (node == NULL) return;

    free(node->children);
    free(node->keys);
    free(node->values);

    free(node);
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
    disk * d;
};


node_type * node_get_righter(BT_type * BT, node_type * node){
    if(node->leaf) return node;

    node_type * child = disk_read(BT->d, node->children[node->size]);
    node_type * node_returned = node_get_righter(BT, child);

    if(child != node_returned) node_free(child);

    return node_returned;
}

node_type * node_get_lefter(BT_type * BT, node_type * node){
    if(node->leaf) return node;

    node_type * child = disk_read(BT->d, node->children[0]);
    node_type * node_returned = node_get_lefter(BT, child);

    if(child != node_returned) node_free(child);

    return node_returned;
}

node_type * node_get_sibling(BT_type * BT, int i, node_type * parent, int right){
    if(parent == NULL) return NULL;

    if(right) i++;
    else i--;

    if(i > parent->size) return NULL;
    if(i < 0) return NULL;

    return disk_read(BT->d, parent->children[i]);
}


BT_type * BT_create(int order){
    BT_type * BT = malloc(sizeof(BT_type));

    BT->order = order;
    BT->root = NULL;
    BT->size = 0;
    BT->d = disk_create("shalom.bin", order);

    return BT;
}

int BT_get_min(BT_type * BT){
    int order = BT->order;
    if(BT->order % 2 != 0) order++;
    return (order / 2) - 1;
}


static void BT_split(BT_type * BT, node_type* parent, int index, node_type* node, int order) {
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
    parent->children[index] = disk_write(BT->d, node, 0);
    parent->children[index+1] = disk_write(BT->d, sibling, 1);

    // Ajusta o tamanho do nó original e do pai
    parent->size++;

    if(parent != BT->root) disk_write(BT->d, parent, 0);

    node_free(sibling);
}


static void BT_insert_nonfull(BT_type * BT, node_type* node, int order, int key, int value) {
    int index = node->size - 1;

    // CASO 1: Nó é folha
    if (node->leaf) {
        // Desloca chaves e valores maiores para a direita
        while (index >= 0 && key <= node->keys[index]) {
            if(node->keys[index] == key) { //verifica antes se não precisa fazer apenas uma atualização de chaves
                node->values[index] = value;
                return;
            }
            node->keys[index + 1] = node->keys[index];
            node->values[index + 1] = node->values[index];
            index--;
        }
        // Insere a nova chave e valor
        node->keys[index + 1] = key;
        node->values[index + 1] = value;
        node->size++;

        if(node != BT->root) disk_write(BT->d, node, 0);
    }
    // CASO 2: Nó é interno
    else {
        // Encontra a posição do filho para descer
        while (index >= 0 && key <= node->keys[index]) {
            if(node->keys[index] == key) { //verifica antes se não precisa fazer apenas uma atualização de chaves
                node->values[index] = value;
                return;
            }
            index--;
        }
        index++;

        node_type * child = disk_read(BT->d, node->children[index]);
        BT_insert_nonfull(BT, child, order, key, value);
        if(child->size == order) BT_split(BT, node, index, child, order);

        node_free(child);
    }
}

void BT_insert(BT_type * BT, int key, int value) {
    if (BT == NULL) return;   // Árvore inválida para inserção

    if (BT->root == NULL) BT->root = node_create(BT->order, 1);

    node_type* root = BT->root;

    BT_insert_nonfull(BT, root, BT->order, key, value);
    if (root->size == BT->order) {
        // Cria novo nó que será a nova raiz
        node_type* new_root = node_create(BT->order, 0);
        BT->root = new_root;
        // O antigo root vira filho[0] da nova raiz
        
        new_root->children[0] = disk_write(BT->d, root, 1);
        node_set_bp(root, new_root->children[0]);
        BT_split(BT, new_root, 0, root, BT->order);

        node_free(root);
    }

}


int BT_search(BT_type * BT, node_type * root, int key) {
    if(root == NULL) return 0;   //Nó invalido para busca

    int value;

    int index = 0, node_size = node_get_size(root);
    while(index < node_size && key > root->keys[index]) index++;

    node_type * new = disk_read(BT->d, root->children[index]);

    if(index < node_size && key == root->keys[index]) value = 1;
    else if(root->leaf) value = 0;  //Não encontrou a chave
    else value = BT_search(BT, new, key);    //Continua a busca nos outros nós

    node_free(new);

    return value;
}


long remove_key(BT_type * BT, node_type * node, int key);

long fix_caso3(BT_type * BT, node_type * parent, int i_parent){
    node_type * node = disk_read(BT->d, parent->children[i_parent]);
    if(node->size >= BT_get_min(BT)){
        node_free(node);
        return parent->pos_binary_file;
    } 

    node_type * left_sibling = node_get_sibling(BT, i_parent, parent, 0);
    node_type * right_sibling = node_get_sibling(BT, i_parent, parent, 1);

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

        disk_write(BT->d, left_sibling, 0);
        disk_write(BT->d, node, 0);

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

        disk_write(BT->d, right_sibling, 0);
        disk_write(BT->d, node, 0);

    } else {
        if(right_sibling == NULL) i_parent--;

        int key_parent = parent->keys[i_parent];

        if(right_sibling){
            node->size++;
            node->children[node->size] = -1;

            node->keys[node->size - 1] = parent->keys[i_parent];
            node->values[node->size - 1] = parent->values[i_parent];
            node_concat(node, right_sibling);   

            disk_write(BT->d, node, 0);
        } else {
            left_sibling->size++;
            left_sibling->children[left_sibling->size] = -1;

            left_sibling->keys[left_sibling->size - 1] = parent->keys[i_parent];
            left_sibling->values[left_sibling->size - 1] = parent->values[i_parent];

            node_concat(left_sibling, node); 

            disk_write(BT->d, left_sibling, 0);
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
            node_type * child = disk_read(BT->d, parent->children[0]);
            int bp = child->pos_binary_file;
            node_free(child);
            node_free(node);
            node_free(parent);
            node_free(left_sibling);
            node_free(right_sibling);
            return bp;
        }
    }

    node_free(node);
    node_free(left_sibling);
    node_free(right_sibling);

    if(parent != BT->root) disk_write(BT->d, parent, 0);
    return parent->pos_binary_file;
}

long remove_key_caso1(BT_type * BT, node_type * node, int key){
    for(int i = 0; i < node->size; i++){
        if(node->keys[i] > key){
            node->keys[i - 1] = node->keys[i];
            node->values[i - 1] = node->values[i];
        } 
    }

    node->size--;

    if(node != BT->root) disk_write(BT->d, node, 0);

    return node->pos_binary_file;
}

long remove_key_caso2(BT_type * BT, node_type * node, int key){
    int i = 0;
    for(; i < node->size; i++) if(key == node->keys[i]) break;

    node_type * r_child = disk_read(BT->d, node->children[i + 1]);
    node_type * l_child = disk_read(BT->d, node->children[i]);

    int bp;

    if(node_get_size(r_child) <= BT_get_min(BT)){
        node_type * righter = node_get_righter(BT, l_child);
        int j = node_get_size(righter) - 1;

        node->keys[i] = righter->keys[j];
        node->values[i] = righter->values[j];

        remove_key(BT, l_child, righter->keys[j]);
        bp = fix_caso3(BT, node, i);

        if(righter != l_child) node_free(l_child);
        node_free(righter);
        node_free(r_child);

    } else {
        node_type * lefter = node_get_lefter(BT, r_child);
        int j = 0;

        node->keys[i] = lefter->keys[j];
        node->values[i] = lefter->values[j];

        remove_key(BT, r_child, lefter->keys[j]);
        bp = fix_caso3(BT, node, i + 1);

        if(lefter != r_child) node_free(r_child);
        node_free(lefter);
        node_free(l_child);
    }

    if(node != BT->root) disk_write(BT->d, node, 0);

    return bp;

}

long remove_key(BT_type * BT, node_type * node, int key){
    int i = 0, bp;
    for(; i < node->size; i++) if(key <= node->keys[i]) break;

    if(i < node->size && key == node->keys[i]){
        if(node->leaf) bp = remove_key_caso1(BT, node, key);
        else bp = remove_key_caso2(BT, node, key);

        return bp;
    }

    node_type * child = disk_read(BT->d, node->children[i]);

    if(node->leaf) bp = node->pos_binary_file;
    else node->children[i] = remove_key(BT, child, key);

    bp = fix_caso3(BT, node, i);

    node_free(child);
    
    return bp;
}

void BT_remove(BT_type * BT, int key){
    long bp = remove_key(BT, BT->root, key);

    if(bp != -1){
        BT->root = disk_read(BT->d, bp);
        node_set_bp(BT->root, -1);
    }
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
                    node_type * add = disk_read(BT->d, node->children[k]);
                    enqueue(queue, add);
                } 
                qtt_filhos += node_get_size(node) + 1;
            }
            if(j != i - 1) printf(" ");
            if(node != BT->root) node_free(node);
        }
        i = qtt_filhos;
        printf("\n");
    }

    queue_free(queue);
}

void BT_free(BT_type * BT){
    node_free(BT->root);
    disk_free(BT->d);
    free(BT);
}

node_type * BT_get_root(BT_type * BT) {
    if(BT != NULL) return BT->root;
    return NULL;
}