#ifndef MATH_PARSE_H
#define MATH_PARSE_H

#include <math.h>

#include "music.h"
#include "types.h"

MathNode* create_node();
void delete_node(MathNode* node);
float evaluate_tree(MathNode* head, float frequency, float time);
MathNode* create_ast(const char** strings, int length);

MathStack* create_stack();
void delete_stack(MathStack* stack);
void stack_push(MathStack* stack, MathNode* node);
MathNode* stack_pop(MathStack* stack);

#endif
