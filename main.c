#include <stdio.h>
#include <stdlib.h>
#include "colors.h"
#include "BT.h"

int main(int argc, char ** argv) {

    if(argc <= 1) {
        printf("%sO diretório do arquivo não foi informado.\n%s", RED, RESET);
        exit(1);
    }

    FILE * file = fopen(argv[1], "r");
    
    if(file == NULL) {
        printf("%sO arquivo informado não existe.\n%s", RED, RESET);
        exit(1);
    }

    int tree_order, number_of_operations;
    fscanf(file, "%d\n", &tree_order);
    fscanf(file, "%d\n", &number_of_operations);

    BT_type * bt = BT_create(tree_order);

    char op = 'X', buffer[256];
    int key = 0, value = 0;

    for(int i = 0; i < number_of_operations; i++) {

        fscanf(file, "%c ", &op);

        switch (op) {

        case 'I':
            fscanf(file, "%d, %d\n", &key, &value);
            BT_insert(bt, key, value);
            break;
        
        case 'R':
            fscanf(file, "%d\n", &key);
            BT_remove(bt, key);
            break;

        case 'B':
            fscanf(file, "%d\n", &key);
            if(BT_search(bt, BT_get_root(bt), key)) printf("O REGISTRO ESTA NA ARVORE!\n");
            else printf("O REGISTRO NAO ESTA NA ARVORE!\n");
            break;

        default:
            i--;
            fscanf(file, "%255[^\n]\n", buffer);
            printf("A operação %s%c%s não pôde ser executada e foi ignorada.\n", CYAN, op, RESET);
            break;
        }
    }

    BT_print(bt);
    BT_free(bt);
    fclose(file);

    return 0;
}