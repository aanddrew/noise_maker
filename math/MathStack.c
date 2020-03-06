#include <stdlib.h>

#include "MathStack.h"

//mathstack
MathStack* MathStack_init() {
    MathStack* stack = malloc(sizeof(MathStack));
    stack->array = malloc(sizeof(MathNode*) * 8);
    stack->num_nodes = 0;
    stack->size = 8;

    return stack;
}

void MathStack_delete(MathStack* stack) {
    free(stack->array);
    free(stack);
}

void MathStack_push(MathStack* stack, MathNode* node) {
    if (stack->num_nodes == stack->size) {
        stack->array = realloc(stack->array, stack->size * 2);
        stack->size *= 2;
    }

    stack->array[stack->num_nodes++] = node;
}

MathNode* MathStack_pop(MathStack* stack) {
    if (stack->num_nodes == 0)
        return NULL;

    return stack->array[--stack->num_nodes];
}

