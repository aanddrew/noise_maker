#ifndef TYPES_H
#define TYPES_H

//must be up here for dependency
typedef float (*sound_func_t)(float, float);

//mathnode
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
    sound_func_t function;
    MathNode* tree;
    char ft;
} MathNode_Value;

struct MathNode_Struct {
    char operation;
    MathNode* left;
    MathNode* right;

    MathNode_Value value;
};

//mathstack
typedef struct {
    MathNode** array;
    size_t num_nodes;
    size_t size;
} MathStack;

//music types
extern const float SCALE_4[];

enum {
    C,
    Cs,
    D,
    Ds,
    E,
    F,
    Fs,
    G,
    Gs,
    A,
    As,
    B,
    NUM_NOTES
};

typedef struct {
    short* buffer;          //data of sample
    int length;             //length of sample in bytes

    int period;             //#bytes until this sample repeats
    int offset;             //where to start playing the sample next (bytes)

    int attack;             //#bytes to start attacking towards
    int fade;               //#bytes to fade out

    //maybe an array of filters on top of it, where filters are functions
} Sample;

typedef struct {
    float attack;
    float decay;
    float sustain;
    float release;
} Envelope;

typedef struct {
    float (*function)(float, float);
} Filter;

typedef struct {
    //sound_func_t function;
    MathNode* tree;
    Envelope env;
    Filter filt;
} Instrument;

typedef struct {
    Instrument ins;
    float* starts;
    float* lengths;
    float* freqs;
    int num_notes;
    int size_notes;
} Track;

#endif
