#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "music/music.h"
#include "sound/wave.h"
#include "globals.h"

float sound_function(float frequency, float t) {
    return sin_function(frequency, exp(-1*(t+2)));
}

float kick(float frequency, float t) {
    float speed = 20.0f;
    return sin_function( frequency/speed, (1.0f / (t*speed + 1.2)));
}

float okay(float frequency, float t) {
    return triangle_function(frequency, 1.0f / (t + 1.2));
}

float growl(float frequency, float t) {
    float base = sin_function(frequency, t) * 0.6;
    int max = (int) (base * 1000.0f);
    printf("%d\n", max);
    float random = 0.0f;
    if (max != 0)
        random = ((float) (rand() % max)) / (1000.0f) * 0.4f;
    //float mod = square_function(frequency, t) * 0.2;
    //float mod = square_function(frequency, t) * 0.2;
    return base + random;
}

float woo(float frequency, float t) {
    frequency += sin_function(frequency, t) * 0.5f;

    float time_in = 0.00f;
    float vol = (t < time_in ? t*t / (time_in * time_in) : 1.0f);

    float actual = sin_function(frequency, t) + (triangle_function(frequency, t) * sin_function(frequency, t + (1.0f/ frequency) * 0.5 ) * 0.2);
    return actual * vol;
}

int main() {
    int num_seconds = 30;
    int size_buffer = num_seconds * WAVE_RATE * 2;

    float* buffer = (float*) malloc(size_buffer * sizeof(float));
    memset(buffer, 0.0f, size_buffer * sizeof(float));
    short* short_buffer = (short*) malloc(size_buffer * sizeof(short));

    Envelope env = {
        0.05f, 0.1, 0.8, 0.6
    };

    Filter filt = {
        null_filter,
        0.15,
    };

    float chords[][3] = {
        {
            NOTE_C4,
            NOTE_E4,
            NOTE_G4,
        },
        {
            NOTE_G4,
            NOTE_B4,
            NOTE_D4 * 2,
        },
        {
            NOTE_A4,
            NOTE_C4,
            NOTE_E4,
        },
        {
            NOTE_F4,
            NOTE_A4,
            NOTE_C4 * 2,
        },
        
        //second bar
        {
            NOTE_C4,
            NOTE_E4,
            NOTE_G4,
        },
        {
            NOTE_G4,
            NOTE_B4,
            NOTE_D4 * 2,
        },
        {
            NOTE_A4,
            NOTE_C4,
            NOTE_E4,
        },
        {
            NOTE_F4,
            NOTE_A4,
            NOTE_C4 * 2,
        },

    };
    int num_chords = 4;
    int size_chords[] = {
        3,
        3,
        3,
        3,
    };

    Instrument ins = {
        woo,
        env,
        filt
    };

    for(int a = 0; a < 3; a++) {
        for(int i = 0; i < num_chords; i++) {
            //for(int j = 0; j < size_chords[i]; j++) {
            for(int j = 0; j < 1; j++) {
                paint(ins , chords[i][j] * 2, buffer, 2.0f * i + a * 8.0f, 2.0f);
            }
        }
    }

    master(buffer, size_buffer);
    discretize(short_buffer, buffer, size_buffer);
    write_to_wave("output.wav", short_buffer, size_buffer);

    free(buffer);
    free(short_buffer);
}
