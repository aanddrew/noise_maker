#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "MathNode.h"
#include "../music/music.h"

//node creation
MathNode* MathNode_init() {
    MathNode* returned = malloc(sizeof(MathNode));
    returned->operation = NULL;
    returned->left = NULL;
    returned->right = NULL;

    returned->value.type = -1;
    returned->value.tree = NULL;
    returned->value.literal = 0;

    return returned;
}

void MathNode_delete(MathNode* node) {
    free(node);
}

float MathNode_evaluate(MathNode* head, float frequency, float time) {
    //leaf node
    if (!head->left && !head->right) {
        if (head->value.type == LITERAL) {
            return head->value.literal;
        }
        else if (head->value.type == FT) {
            return head->value.ft == 'f' ? frequency : time;
        }
        else {
            return MathNode_evaluate(head->value.tree, frequency, time);
        }
    }

    float left = MathNode_evaluate(head->left, frequency, time);
    float right = MathNode_evaluate(head->right, frequency, time);
    if (!strcmp(head->operation, "sin")) {
        return sin_function(left, right);
    }
    else if (!strcmp(head->operation, "square")) {
        return square_function(left, right);
    }
    else if (!strcmp(head->operation, "triangle")) {
        return triangle_function(left, right);
    }
    else if (!strcmp(head->operation, "saw")) {
        return saw_function(left, right);
    }

    switch(head->operation[0]) {
        case '+': return left + right; break;
        case '*': return left * right; break;
        case '-': return left - right; break;
        case '/': return left / right; break;
        case '^': return pow(left, right); break;

        default: return (0.0 / 0.0);
    }
}

