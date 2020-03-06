#include <stdlib.h>
#include <string.h>

#include "../globals.h"
#include "../math/MathNode.h"
#include "Instrument.h"

static const Envelope default_env = {
    .attack = 0.05f,
    .decay = 0.0f,
    .sustain = 1.0f,
    .release = 0.05f
};

Instrument* Instrument_init(const char* name) {
    Instrument* newins = malloc(sizeof(Instrument));
    newins->name = strdup(name);
    newins->envelope = default_env;
    return newins;
}

void Instrument_delete(Instrument* ins) {
    free(ins->name);
    free(ins);
}

void Instrument_paint(Instrument* ins, float frequency, float* canvas, float start_t, float total_t) {
    int num_frames = WAVE_RATE * (total_t + ins->envelope.release);
    int start_frame = WAVE_RATE * start_t;

    for (int frame = start_frame; frame < num_frames + start_frame; frame++) {
        float t = (float) (frame - start_frame) / WAVE_RATE;

        float vol_mod = ins->envelope.sustain;
        if (t < ins->envelope.attack) {
            vol_mod = t / ins->envelope.attack;
        }
        else if (t < ins->envelope.attack + ins->envelope.decay) {
            float diff = 1.0f - ins->envelope.sustain;
            float sub = diff * ((t - ins->envelope.attack) / ins->envelope.decay);
            vol_mod = 1.0f - sub;
        }
        else if (t >= total_t) {
            vol_mod = 1.0f - ((t - total_t) / ins->envelope.release);
            vol_mod *= ins->envelope.sustain;
        }

        float value = MathNode_evaluate(ins->tree->head, frequency, t) * vol_mod;
        canvas[2*frame + 0] += value;
        canvas[2*frame + 1] += value;
    }
}

