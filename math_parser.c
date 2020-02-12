#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "math_parser.h"
#include "parser.h"

static int is_operand(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

//tree stuff
MathNode* create_ast(const char** strings, int length) {
    MathStack* stack = create_stack();

    for(int i = 0; i < length; i++) {
        int should_pop =  0;
        MathNode* newnode = create_node();
        if (is_operand(strings[i][0])) {
            newnode->operation = strings[i][0];
            should_pop = 1;
        }
        else {
            if (!strcmp(strings[i], "sin")) {
                should_pop = 1;
                newnode->value.type = FUNCTION;
                newnode->value.function = sin_function;
            }
            else if (!strcmp(strings[i], "saw")) {
                should_pop = 1;
                newnode->value.type = FUNCTION;
                newnode->value.function = saw_function;
            }
            else if (!strcmp(strings[i], "triangle")) {
                should_pop = 1;
                newnode->value.type = FUNCTION;
                newnode->value.function = triangle_function;
            }
            else if (!strcmp(strings[i], "square")) {
                should_pop = 1;
                newnode->value.type = FUNCTION;
                newnode->value.function = square_function;
            }
            else {
                //f,t, subtree or literal value
                //check literal first
                float literal;
                if(sscanf(strings[i], "%f", &literal)) {
                    newnode->value.type = LITERAL;
                    newnode->value.literal = literal;
                }
                else {
                    //f or t?
                    if (!strcmp(strings[i], "f")) {
                        newnode->value.type = FT;
                        newnode->value.ft = 'f';
                    }
                    else if (!strcmp(strings[i], "t")) {
                        newnode->value.type = FT;
                        newnode->value.ft = 't';
                    }
                    else {
                        newnode->value.type = TREE;
                        newnode->value.tree = get_function_tree(strings[i]);
                    }
               }
            }

            if (should_pop) {
                MathNode* right = stack_pop(stack);
                MathNode* left = stack_pop(stack);
                if (!right || !left) {
                    printf("Error building AST!!!\n");
                    return NULL;
                }
                newnode->left = left;
                newnode->right = right;
            }

            stack_push(stack, newnode);
        }
    }

    //build up tree from this stack
    MathNode* returned = stack_pop(stack);
    delete_stack(stack);
    return returned;
}

void delete_ast(MathNode* head) {
    if (head->left)
        delete_ast(head->left);
    if (head->right)
        delete_ast(head->right);

    free(head);
}

//node creation
MathNode* create_node() {
    MathNode* returned = malloc(sizeof(MathNode));
    returned->operation = '\0';
    returned->left = NULL;
    returned->right = NULL;

    returned->value.type = -1;
    returned->value.function = NULL;
    returned->value.tree = NULL;
    returned->value.literal = 0;

    return returned;
}

void delete_node(MathNode* node) {
    free(node);
}

float evaluate_tree(MathNode* head, float frequency, float time) {
    //leaf node
    if (!head->left && !head->right) {
        if (head->value.type == LITERAL) {
            return head->value.literal;
        }
        else if (head->value.type == FT) {
            return head->value.ft == 'f' ? frequency : time;
        }
        else {
            return evaluate_tree(head->value.tree, frequency, time);
        }
    }

    float left = evaluate_tree(head->left, frequency, time);
    float right = evaluate_tree(head->right, frequency, time);
    if (head->value.type == FUNCTION) {
        return head->value.function(left, right);
    }
    switch(head->operation) {
        case '+': return left + right; break;
        case '*': return left * right; break;
        case '-': return left - right; break;
        case '/': return left / right; break;
        case '^': return pow(left, right); break;

        default: return (0.0 / 0.0);
    }
}

//mathstack
MathStack* create_stack() {
    MathStack* stack = malloc(sizeof(MathStack));
    stack->array = malloc(sizeof(MathNode*) * 8);
    stack->size = 8;

    return stack;
}

void delete_stack(MathStack* stack) {
    free(stack->array);
    free(stack);
}

void stack_push(MathStack* stack, MathNode* node) {
    if (stack->num_nodes == stack->size) {
        stack->array = realloc(stack->array, stack->size * 2);
        stack->size *= 2;
    }

    stack->array[stack->num_nodes++] = node;
}

MathNode* stack_pop(MathStack* stack) {
    if (stack->num_nodes == 0)
        return NULL;

    return stack->array[--stack->num_nodes];
}

