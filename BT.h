#ifndef BT_H
#define BT_H

#include <stdio.h>

typedef struct BT BT_type;

BT_type * create_BT(int order);

void insert_BT(BT_type * BT, int key, int value);

int search_BT(BT_type * BT, int key);

void remove_BT(BT_type * BT, int key);

void free_BT(BT_type * BT);

#endif