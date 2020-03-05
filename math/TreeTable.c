#include "TreeTable.h"

#include <string.h>
#include <stdlib.h>

TreeTable* TreeTable_init() {
    TreeTable* newtable = malloc(sizeof(TreeTable));
    newtable->size = 4;
    newtable->num = 0;
    newtable->trees = malloc(sizeof(MathTree*) * newtable->size);

    return newtable;
}

void TreeTable_delete(TreeTable* table) {
    free(table->trees);
    free(table);
}

void TreeTable_insert(TreeTable* table, MathTree* tree) {
    if (table->num == table->size) {
        table->size *= 2;
        table->trees = realloc(table->trees, table->size);
    }

    table->trees[table->num] = tree;
    table->num++;
}

MathTree* TreeTable_lookup(TreeTable* table, const char* name) {
    for(int i = 0; i < table->num; i++) {
        if (!strcmp(table->trees[i]->name, name)) {
            return table->trees[i];
        }
    }
    return NULL;
}
