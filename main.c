#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "music.h"
#include "wave.h"
#include "globals.h"
#include "parser.h"

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

const Envelope default_env = {
    .attack = 0.05f,
    .decay = 0.0f,
    .sustain = 1.0f,
    .release = 0.05f
};

const Filter default_filt = {
    .function = null_filter,
};

int main(int argc, char** argv) {
    if (argc == 1) {
        printf("No File Provided - usage: noise_maker file.nm\n");
        exit(0);
    }
    else {
        printf("Compiling %s...\n", argv[1]);
    }

    int num_seconds = 30;
    int size_buffer = num_seconds * WAVE_RATE * 2;

    float* buffer = (float*) malloc(size_buffer * sizeof(float));
    memset(buffer, 0.0f, size_buffer * sizeof(float));
    short* short_buffer = (short*) malloc(size_buffer * sizeof(short));

    int num_tracks, tempo;
    Track* tracks = parse_song(argv[1], &num_tracks, &tempo);

    for(int i = 0; i < num_tracks; i++) {
        paint_track(tracks[i], buffer);
    }

    master(buffer, size_buffer);
    discretize(short_buffer, buffer, size_buffer);
    write_to_wave("output.wav", short_buffer, size_buffer);

    free(buffer);
    free(short_buffer);

    for(int i = 0; i < num_tracks; i++) {
        destroy_track(&tracks[i]);
    }

    free(tracks);
}
