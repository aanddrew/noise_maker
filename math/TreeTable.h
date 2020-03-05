#ifndef TREETABLE_H
#define TREETABLE_H

#include "../math/MathTree.h"

typedef struct {
    MathTree** trees;
    int num;
    int size;
} TreeTable;

TreeTable* TreeTable_init();
void TreeTable_delete(TreeTable* table);

void TreeTable_insert(TreeTable* table, MathTree* tree);
MathTree* TreeTable_lookup(TreeTable* table, const char* name);

#endif
