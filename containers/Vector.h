#ifndef VECTOR_H
#define VECTOR_H

//#include "../math/MathTree.h"

typedef struct {
    void** array;
    int num;
    int size;
} Vector;

Vector* Vector_init();
void Vector_delete(Vector* vec);

void Vector_push(Vector* vec, void* element);
void* Vector_get(Vector* vec, unsigned int index);

#endif
