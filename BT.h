#ifndef BT_H
#define BT_H

#include <stdio.h>

typedef struct BT BT_type;

BT_type * BT_create(int order);

void BT_insert(BT_type * BT, int key, int value);

int BT_search(BT_type * BT, int key);

void BT_remove(BT_type * BT, int key);

void BT_free(BT_type * BT);

#endif