#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "MathTree.h"
#include "TreeTable.h"
#include "MathStack.h"

static TreeTable* tree_table = NULL;

MathTree* MathTree_lookup(const char* name) {
    return TreeTable_lookup(tree_table, name);
}

static int is_operand(const char* str) {
    switch(str[0]) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '^':
            return 1;
        break;
    }
    if (!strcmp(str, "sin") ||
        !strcmp(str, "square") || 
        !strcmp(str, "triangle") ||
        !strcmp(str, "saw")) {
        return 1;
    }

    return 0;
}

MathTree* MathTree_init(const char* name, const char** strings, int length) {
    MathStack* stack = MathStack_init();

    for(int i = 0; i < length; i++) {
        int should_pop =  0;
        MathNode* newnode = MathNode_init();
        if (is_operand(strings[i])) {
            newnode->operation = strdup(strings[i]);
            should_pop = 1;
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
                    newnode->value.tree = TreeTable_lookup(tree_table, strings[i])->head;
                }
           }
        }
        if (should_pop) {
            MathNode* right = MathStack_pop(stack);
            MathNode* left = MathStack_pop(stack);
            if (!right || !left) {
                printf("Error building AST!!!\n");
                return NULL;
            }
            newnode->left = left;
            newnode->right = right;
        }

        MathStack_push(stack, newnode);
    }

    //build up tree from this stack
    MathNode* head = MathStack_pop(stack);
    MathStack_delete(stack);
    
    MathTree* newtree = malloc(sizeof(MathTree));
    newtree->name = strdup(name);
    newtree->head = head;
    
    if (!tree_table) {
        tree_table = TreeTable_init();
    }
    TreeTable_insert(tree_table, newtree);

    return newtree;
}

static void delete_ast(MathNode* head) {
    if (head->left)
        delete_ast(head->left);
    if (head->right)
        delete_ast(head->right);
    free(head);
}

void MathTree_delete(MathTree* tree) {
    delete_ast(tree->head);
    free(tree->name);
    free(tree);
}
