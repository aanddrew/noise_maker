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
} Track;

void Track_init(Track* track);
void Track_delete(Track* track);

void Track_paint(Track* track, float* canvas);
void Track_insert_note(Track* track, float freq, float start, float length);

#endif
