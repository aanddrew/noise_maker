#include <alsa/asoundlib.h>
#include <math.h>
#include "sound_engine.h"
#include "music.h"
#include "wave.h"

int main() {
    if(!init_sound_engine()) {
        fprintf(stderr, "Error initializing sound engine, exiting!\n");
        return -1;
    }

    int num_seconds = 30;
    int size_buffer = num_seconds * rate * 2;

    float* buffer = (float*) malloc(size_buffer * sizeof(float));
    memset(buffer, 0, size_buffer * sizeof(float));
    short* short_buffer = (short*) malloc(size_buffer * sizeof(short));

    float chord[] = {
        NOTE_F4s / 2,
        NOTE_A4 / 2,
        NOTE_D4,
    };
    int size_chord = sizeof(chord)/sizeof(float);

    for(int i = 0; i < size_chord; i++) {
        paint(triangle_function, chord[i], buffer, 0, 2.0f);
    }
    for(int i = 0; i < size_chord; i++) {
        paint(triangle_function, chord[i] * 1.1, buffer, 2.0f, 2.0f);
    }
    for(int i = 0; i < size_chord; i++) {
        paint(triangle_function, chord[i] * 0.93, buffer, 4.0f, 2.0f);
    }
    for(int i = 0; i < size_chord; i++) {
        paint(triangle_function, chord[i] * 1.2, buffer, 6.0f, 2.0f);
    }

    master(buffer, size_buffer);
    discretize(short_buffer, buffer, size_buffer);
    write_to_wave("output.wav", short_buffer, size_buffer);

    snd_pcm_prepare(pcm_handle);
    int pcm_return = 0;
    int offset = 0;
    while (offset < size_buffer / 2) {
        if (pcm_return > 100)
            offset += pcm_return * 2;

        printf("%d, %d\n", offset, pcm_return);

        snd_pcm_prepare(pcm_handle);
        while((pcm_return = snd_pcm_writei(pcm_handle, &(short_buffer[offset]) , frames_per_buffer)) < 0) {
            snd_pcm_prepare(pcm_handle);
            printf("Buffer Underrun\n");
        }
    }

    free(buffer);
    free(short_buffer);
}
