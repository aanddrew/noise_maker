#ifndef MUSIC_H
#define MUSIC_H

#include <math.h>
#include <stdlib.h>

#include "Instrument.h"
#include "Track.h"

#define NOTE_C4  261.63f
#define NOTE_C4s 277.18f
#define NOTE_D4  293.66f
#define NOTE_D4s 311.13f
#define NOTE_E4  329.63f
#define NOTE_F4  349.23f
#define NOTE_F4s 369.99f
#define NOTE_G4  392.00f
#define NOTE_G4s 415.30f
#define NOTE_A4  440.00f
#define NOTE_A4s 466.16f
#define NOTE_B4  493.88f

extern const float SCALE_4[12];

enum {
    C = 0,
    D = 2,
    E = 4,
    F = 5,
    G = 7,
    A = 9,
    B = 11
};

void discretize(short* short_buffer, float* original, int length);
void master(float* canvas, int canvas_length);

float sin_function(float frequency, float t);
float square_function(float frequency, float t);
float saw_function(float frequency, float t);
float triangle_function(float frequency, float t);

float null_filter(float value, float t);
float quantize_filter(float value, float t, float step_size);

#endif 
