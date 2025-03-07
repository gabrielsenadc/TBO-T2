#include <stdlib.h>
#include "BT.h"
#include "queue.h"

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

int node_get_size(node_type * node){
    if(node == NULL) return 0;
    return node->size;
}

void node_free(node_type * node){
    if(node == NULL) return;

    if(node->size > 0) for(int i = 0; i <= node->size; i++) node_free(node->children[i]);
    free(node->children);

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

node_type * node_get_sibling(node_type * node, int right){
    node_type * parent = node->parent;
    if(parent == NULL) return NULL;

    int key = node->keys[0];

    int i = 0;
    for(; i < parent->size; i++) if(key < parent->keys[i]) break;

    if(right) i++;
    else i--;

    if(i >= parent->size) return NULL;

    return parent->children[i];
}

void node_concat(node_type * dest, node_type * src);

void node_print(node_type * node){
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

void remove_key_caso1(BT_type * BT, node_type * node, int key){
    for(int i = 0; i < node->size; i++){
        if(node->keys[i] > key){
            node->keys[i - 1] = node->keys[i];
            node->values[i - 1] = node->values[i];
        } 
    }

    node->size--;
}

void remove_key_caso2(BT_type * BT, node_type * node, int key){
    int i = 0;
    for(; i < node->size; i++) if(key == node->keys[i]) break;

    if(get_size_node(node->children[i]) > BT->order/2){
        node_type * righter = node_get_righter(node->children[i]);
        int j = get_size_node(righter) - 1;

        node->keys[i] = righter->keys[j];
        node->values[i] = righter->values[j];

        remove_key(BT, node->children[i], righter->keys[j]);

    } else if(get_size_node(node->children[i + 1]) > BT->order/2){
        node_type * lefter = node_get_lefter(node->children[i + 1]);
        int j = 0;

        node->keys[i] = lefter->keys[j];
        node->values[i] = lefter->values[j];

        remove_key(BT, node->children[i + 1], lefter->keys[j]);

    } else printf("C");

}

void remove_key_caso3(BT_type * BT, node_type * node, int key){
    node_type * left_sibling = node_get_sibling(node, 0);
    node_type * right_sibling = node_get_sibling(node, 1);

    node_type * parent = node->parent;

    if(node_get_size(left_sibling) > BT->order/2){
        int i_parent = 0;
        for(; i_parent < parent->size; i_parent++) if(key < parent->keys[i_parent]) break;
        i_parent--;

        for(int i = node->size - 1; i > 0; i--){
            if(node->keys[i] <= key){
                node->keys[i] = node->keys[i - 1];
                node->values[i] = node->values[i - 1];
            }
        }

        int i_sibling = node_get_size(left_sibling) - 1;
        
        node->keys[0] = parent->keys[i_parent];
        node->values[0] = parent->values[i_parent];

        parent->keys[i_parent] = left_sibling->keys[i_sibling];
        parent->values[i_parent] = left_sibling->values[i_sibling];

        remove_key(BT, left_sibling, left_sibling->keys[i_sibling]);

    } else if(node_get_size(left_sibling) > BT->order/2){
        int i_parent = 0;
        for(; i_parent < parent->size; i_parent++) if(key < parent->keys[i_parent]) break;

        for(int i = 0; i < node->size; i++){
            if(node->keys[i] > key){
                node->keys[i - 1] = node->keys[i];
                node->values[i - 1] = node->values[i];
            } 
        }

        int i_sibling = 0;
        
        node->keys[node->size - 1] = parent->keys[i_parent];
        node->values[node->size - 1] = parent->values[i_parent];

        parent->keys[i_parent] = right_sibling->keys[i_sibling];
        parent->values[i_parent] = right_sibling->values[i_sibling];

        remove_key(BT, right_sibling, right_sibling->keys[i_sibling]);
    } else {
        int i_parent = 0;
        for(; i_parent < parent->size; i_parent++) if(key < parent->keys[i_parent]) break;
        int key_parent = parent->keys[i_parent];

        for(int i = 0; i < node->size; i++){
            if(node->keys[i] > key){
                node->keys[i - 1] = node->keys[i];
                node->values[i - 1] = node->values[i];
            } 
        }

        node->keys[node->size - 1] = parent->keys[i_parent];
        node->values[node->size - 1] = parent->values[i_parent];

        node_concat(node, right_sibling);   
        node_free(right_sibling);

        for(int i = 0; i < parent->size; i++){
            if(parent->keys[i] > key_parent){
                parent->keys[i - 1] = parent->keys[i];
                parent->values[i - 1] = parent->values[i];
                parent->children[i] = parent->children[i + 1];
            } 
        }

        parent->size--;
    }
}

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

void BT_remove(BT_type * BT, int key){
    remove_key(BT, BT->root, key);
}

void BT_print(BT_type * BT){
    queue_type * queue = queue_create();

    enqueue(queue, BT->root);
    int i = 1;

    node_type * node;
    while(!empty(queue)){
        int qtt_filhos = 0;
        for(int j = 0; j < i; j++){
            node = dequeue(queue);
            node_print(node);
            if(node_get_size(node) > 0) {
                for(int i = 0; i <= node_get_size(node); i++) enqueue(queue, node->children[i]);
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