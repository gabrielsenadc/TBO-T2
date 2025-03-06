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



void free_BT(BT_type * BT){
    free_node(BT->root);
    free(BT);
}