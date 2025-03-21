#include <stdio.h>
#include <stdlib.h>
#include "colors.h"
#include "BT.h"

int main(int argc, char ** argv) {

    BT_type * bt = BT_create(5);

    for(int i = 1; i <= 18; i++) BT_insert(bt, i, i);

    //BT_remove(bt, 15);

    BT_print(bt);
    BT_free(bt);

    return 0;
}