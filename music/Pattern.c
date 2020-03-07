#include <stdlib.h>

#include "Pattern.h"

Pattern* Pattern_init() {
    Pattern* pattern = malloc(sizeof(Pattern));
    pattern->num_notes = 0;
    pattern->size_notes = 2;
    pattern->freqs   = malloc(sizeof(float) * pattern->size_notes);
    pattern->starts  = malloc(sizeof(float) * pattern->size_notes);
    pattern->lengths = malloc(sizeof(float) * pattern->size_notes);

    return pattern;
}

void Pattern_delete(Pattern* pattern) {
    free(pattern->freqs);
    free(pattern->starts);
    free(pattern->lengths);
    free(pattern);
}

void Pattern_paint(Pattern* pattern, float* canvas, float offset) {
    for(int i = 0; i < pattern->num_notes; i++) {
        Instrument_paint(pattern->ins, pattern->freqs[i], canvas, pattern->starts[i] + offset, pattern->lengths[i]);
    }
}

void Pattern_insert_note(Pattern* pattern, float freq, float start, float length) {
    if (pattern->num_notes >= pattern->size_notes) {
        //resizing time
        pattern->size_notes *= 2;
        int size_mem = pattern->size_notes * sizeof(float);

        pattern->freqs = realloc(pattern->freqs, size_mem);
        pattern->starts = realloc(pattern->starts, size_mem);
        pattern->lengths = realloc(pattern->lengths, size_mem);
    }

    pattern->freqs  [pattern->num_notes] = freq;
    pattern->starts [pattern->num_notes] = start;
    pattern->lengths[pattern->num_notes] = length;
    pattern->num_notes++;
}

