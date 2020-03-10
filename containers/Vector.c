#include "Vector.h"
#include <stdlib.h>

Vector* Vector_init() {
    Vector* newvec = malloc(sizeof(Vector));
    newvec->size = 4;
    newvec->num = 0;
    newvec->array = malloc(sizeof(void*) * newvec->size);

    return newvec;
}

void Vector_delete(Vector* vec) {
    free(vec->array);
    free(vec);
}

void Vector_push(Vector* vec, void* element) {
    if (vec->num == vec->size) {
        vec->size *= 2;
        vec->array = realloc(vec->array, sizeof(void*) * vec->size);
    }

    vec->array[vec->num] = element;
    vec->num++;
}

void* Vector_get(Vector* vec, int index) {
    if (index >= vec->num || index < 0) {
        return NULL;
    }
    return vec->array[index];
}
