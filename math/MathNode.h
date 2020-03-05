#ifndef MATHNODE_H
#define MATHNODE_H

struct MathNode_Struct;
typedef struct MathNode_Struct MathNode;

enum VALUETYPES {
    LITERAL,
    FUNCTION,
    TREE,
    FT,         //frequency and time
};

typedef struct {
    int type;

    float literal;
    char ft;
    MathNode* tree;
} MathNode_Value;

struct MathNode_Struct {
    char* operation;
    MathNode* left;
    MathNode* right;

    MathNode_Value value;
};

MathNode* MathNode_init();
void MathNode_delete(MathNode* head);
float MathNode_evaluate(MathNode* head, float frequency, float time);

#endif
