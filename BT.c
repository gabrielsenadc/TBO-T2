#include <stdlib.h>
#include "BT.h"

typedef struct node node_type;

struct node {
    int size;     //amount of keys in this node
    int leaf;     //flag if node is a leaf  
    int pos_binary_file;     //node position in the binary file   
    int * keys;     //array of keys
    int * values;     //array o values, each indexed by a key
    node_type ** children;     //array of children
    node_type * parent;     //pointer to the node parent
};

int get_size_node(node_type * node){
    return node->size;
}

void free_node(node_type * node){
    if(node == NULL) return;

    for(int i = 0; i <= node->size; i++) free_node(node->children[i]);
    free(node->children);

    free(node);
}

struct BT {
    int order;     //BT order
    node_type * root;     //poiter to the root node
    int size;     //total number of nodes in this BT
};


BT_type * create_BT(int order){
    BT_type * BT = malloc(sizeof(BT_type));

    BT->order = order;
    BT->root = NULL;
    BT->size = 0;

    return BT;
}

void remove_key_caso1(BT_type * BT, node_type * node, int key){
    for(int i = 0; i < node->size; i++){
        if(node->keys[i] > key) node->keys[i - 1] = node->keys[i];
    }

    node->size--;
}

void remove_key_caso2(BT_type * BT, node_type * node, int key){
    int i = 0;
    for(; i < node->size; i++) if(key == node->keys[i]) break;

    if(get_size_node(node->children[i]) > BT->order/2) printf("A");
    else if(get_size_node(node->children[i + 1]) > BT->order/2) printf("B");
    else printf("C");

}

void remove_key_caso3(BT_type * BT, node_type * node, int key);

void remove_key(BT_type * BT, node_type * node, int key){
    int i = 0;
    for(; i < node->size; i++) if(key <= node->keys[i]) break;

    if(i < node->size && key == node->keys[i]){
        if(node->leaf){
            if(node->size > BT->order/2) remove_key_caso1(BT, node, key);
            else remove_key_caso3(BT, node, key);
        } else {
            remove_key_caso2(BT, node, key);
        }

        return;
    }

    if(node->leaf) printf("NAO EXISTE");
    else remove_node(BT, node->children[i], key);
}



void free_BT(BT_type * BT){
    free_node(BT->root);
    free(BT);
}