#ifndef MUSIC_H
#define MUSIC_H

#include <math.h>
#include <stdlib.h>

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

typedef float (*sound_func_t)(float, float);

extern const float SCALE_4[];

enum {
    C,
    Cs,
    D,
    Ds,
    E,
    F,
    Fs,
    G,
    Gs,
    A,
    As,
    B,
    NUM_NOTES
};

typedef struct {
    short* buffer;          //data of sample
    int length;             //length of sample in bytes

    int period;             //#bytes until this sample repeats
    int offset;             //where to start playing the sample next (bytes)

    int attack;             //#bytes to start attacking towards
    int fade;               //#bytes to fade out

    //maybe an array of filters on top of it, where filters are functions
} Sample;

typedef struct {
    float attack;
    float decay;
    float sustain;
    float release;
} Envelope;

typedef struct {
    float (*function)(float, float);
} Filter;

typedef struct {
    sound_func_t function;
    Envelope env;
    Filter filt;
} Instrument;

typedef struct {
    Instrument ins;
    float* starts;
    float* lengths;
    float* freqs;
    int num_notes;
    int size_notes;
} Track;

void paint_track(Track track, float* canvas);
void paint_ins(Instrument ins, float frequency, float* canvas, float start_t, float total_t);
void discretize(short* short_buffer, float* original, int length);
void master(float* canvas, int canvas_length);

void init_track(Track* track);
void destroy_track(Track* track);
void insert_note_into_track(Track* track, float freq, float start, float length); 

float sin_function(float frequency, float t);
float square_function(float frequency, float t);
float saw_function(float frequency, float t);
float triangle_function(float frequency, float t);

float null_filter(float value, float t);
float quantize_filter(float value, float t, float step_size);

extern const Envelope default_env;
extern const Filter default_filt;

#endif 
