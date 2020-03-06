#ifndef MATHTREE_H
#define MATHTREE_H

#include "MathNode.h"

typedef struct {
    char* name;
    MathNode* head;
} MathTree;

MathTree* MathTree_init(const char* name, const char** strings, int length);
void MathTree_delete(MathTree* tree);

MathTree* MathTree_lookup(const char* name);
void MathTree_cleanup();

#endif
