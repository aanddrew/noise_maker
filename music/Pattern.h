#ifndef TRACK_H
#define TRACK_H

#include "Instrument.h"

typedef struct {
    Instrument* ins;
    float* starts;
    float* lengths;
    float* freqs;
    int num_notes;
    int size_notes;
} Pattern;

Pattern* Pattern_init();
void Pattern_delete(Pattern* pattern);

void Pattern_paint(Pattern* pattern, float* canvas, float offset);
void Pattern_insert_note(Pattern* pattern, float freq, float start, float length);

#endif
