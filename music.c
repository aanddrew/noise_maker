#include <stdio.h>

#include "music.h"

#define SHORT_MAX 0x7fff

#include "../globals.h"

const float SCALE_4[] = {
    NOTE_C4,
    NOTE_C4s,
    NOTE_D4,
    NOTE_D4s,
    NOTE_E4,
    NOTE_F4,
    NOTE_F4s,
    NOTE_G4,
    NOTE_G4s,
    NOTE_A4,
    NOTE_A4s,
    NOTE_B4,
};

float sin_function(float frequency, float t) {
    return sin(t * frequency * 2*M_PI * 2);
}

float square_function(float frequency, float t) {
    int high = (int)((t*2*2) * frequency) % 2 == 0;
    return (high ? 1.0f : -1.0f);
}

float saw_function(float frequency, float t) {
    float thing = t * frequency * 2.0f;
    float val = fmod(thing, 1.0f) * 2 - 1;
    return val;
}

float triangle_function(float frequency, float t) {
    float thing = t * frequency * 2.0f;
    float val = fmod(thing, 2.0f);
    if (val > 1.0f)
        val = 2.0f - val;
    return val * 2 - 1;
}

float null_filter(float value, float t, ...) {
    return value;
}

float quantize_filter(float value, float t, float step_size) {
    return value - fmod(value, step_size);
}

void paint(Instrument ins, float frequency, float* canvas, float start_t, float total_t) {
    int num_frames = WAVE_RATE * (total_t + ins.env.release);
    int start_frame = WAVE_RATE * start_t;

    for (int frame = start_frame; frame < num_frames + start_frame; frame++) {
        float t = (float) (frame - start_frame) / WAVE_RATE;

        float vol_mod = ins.env.sustain;
        if (t < ins.env.attack) {
            vol_mod = t / ins.env.attack;
        }
        else if (t < ins.env.attack + ins.env.decay) {
            float diff = 1.0f - ins.env.sustain;
            float sub = diff * ((t - ins.env.attack) / ins.env.decay);
            vol_mod = 1.0f - sub;
        }
        else if (t >= total_t) {
            vol_mod = 1.0f - ((t - total_t) / ins.env.release);
            vol_mod *= ins.env.sustain;
        }

        float value = ins.function(frequency, t) * vol_mod;
        value = ins.filt.function(value, t, ins.filt.arg);
        canvas[2*frame + 0] += value;
        canvas[2*frame + 1] += value;
    }
}

void master(float* buffer, int length) {
    float highest_value = 1.0f;
    for(int i = 0; i < length; i++) {
        if (buffer[i] > highest_value)
            highest_value = buffer[i];
    }
    
    float divisor = (highest_value * 1.1 > highest_value + 1 ? highest_value + 1 : highest_value * 1.1);
    for(int i = 0; i < length; i++) {
        buffer[i] /= divisor;
    }
}

void discretize(short* short_buffer, float* original, int length) {
    for(int i = 0; i < length; i++) {
        short_buffer[i] = (short) (original[i] * SHORT_MAX);
    }
}
