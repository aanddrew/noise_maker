#include "Track.h"

#include <stdlib.h>

Track* Track_init() {
    Track* newtrack = malloc(sizeof(Track));
    newtrack->patterns = Vector_init();
    newtrack->offsets = Vector_init();

    return newtrack;
}

void Track_delete(Track* track) {
    for(int i = 0; i < track->offsets->num; i++) {
        Vector* offset_vec_i = (Vector*) Vector_get(track->offsets, i);
        for(int j = 0; j < offset_vec_i->num; j++) {
            free(Vector_get(offset_vec_i, j));
        }
        Vector_delete(offset_vec_i);
    }
    Vector_delete(track->offsets);
}

void Track_paint(Track* track, float* canvas) {
    for(int i = 0; i < track->patterns->num; i++) {
        Pattern* current_pattern = (Pattern*) Vector_get(track->patterns, i);
        Vector* current_offsets = (Vector*) Vector_get(track->offsets, i);
        for(int j = 0; j < current_offsets->num; j++) {
            Pattern_paint(current_pattern, canvas, *((float*) Vector_get(current_offsets, j)));
        }
    }
}

void Track_add_pattern(Track* track, Pattern* pattern) {
    Vector_push(track->patterns, pattern);
    Vector_push(track->offsets, Vector_init());
}

void Track_insert_offset(Track* track, Pattern* pattern, float offset) {
    int in_vector = 0;
    for(int i = 0; i < track->patterns->num; i++) {
        Pattern* current_pattern = (Pattern*) Vector_get(track->patterns, i);
        Vector* current_offsets = (Vector*) Vector_get(track->offsets, i);
        if (current_pattern == pattern) {
            float* val = malloc(sizeof(float));
            *val = offset;
            Vector_push(current_offsets, val);

            in_vector = 1;
            break;
        }
    }

    if (!in_vector) {
        Vector_push(track->patterns, pattern);
        Vector_push(track->offsets, Vector_init());
    
        float* val = malloc(sizeof(float));
        *val = offset;
        Vector_push(Vector_get(track->offsets, track->offsets->num - 1), val);
    }
}

