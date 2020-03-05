#ifndef MATHSTACK_H
#define MATHSTACK_H

#include "MathNode.h"

typedef struct {
    MathNode** array;
    size_t num_nodes;
    size_t size;
} MathStack;

MathStack* MathStack_init();
void MathStack_delete(MathStack* stack);
void MathStack_push(MathStack* stack, MathNode* node);
MathNode* MathStack_pop(MathStack* stack);

#endif
