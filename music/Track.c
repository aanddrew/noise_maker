#include <stdlib.h>

#include "Track.h"

void Track_init(Track* track) {
    track->size_notes = 2;
    track->freqs   = malloc(sizeof(float) * track->size_notes);
    track->starts  = malloc(sizeof(float) * track->size_notes);
    track->lengths = malloc(sizeof(float) * track->size_notes);
}

void Track_delete(Track* track) {
    free(track->freqs);
    free(track->starts);
    free(track->lengths);
}

void Track_paint(Track* track, float* canvas) {
    for(int i = 0; i < track->num_notes; i++) {
        Instrument_paint(track->ins, track->freqs[i], canvas, track->starts[i], track->lengths[i]);
    }
}

void Track_insert_note(Track* track, float freq, float start, float length) {
    if (track->num_notes >= track->size_notes) {
        //resizing time
        track->size_notes *= 2;
        int size_mem = track->size_notes * sizeof(float);

        track->freqs = realloc(track->freqs, size_mem);
        track->starts = realloc(track->starts, size_mem);
        track->lengths = realloc(track->lengths, size_mem);
    }

    track->freqs  [track->num_notes] = freq;
    track->starts [track->num_notes] = start;
    track->lengths[track->num_notes] = length;
    track->num_notes++;
}

