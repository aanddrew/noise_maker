#ifndef INSTRUMENT_H
#define INSTRUMENT_H

#include "../math/MathTree.h"

typedef struct {
    char* name;
    float attack;
    float decay;
    float sustain;
    float release;
} Envelope;

extern const Envelope default_env;

typedef struct {
    char* name;

    MathTree* tree;
    Envelope envelope;
    MathTree* filter;
} Instrument;

Instrument* Instrument_init(const char* name);
void Instrument_delete(Instrument* ins);

void Instrument_paint(Instrument* ins, float frequency, float* canvas, float start_t, float total_t);

#endif
