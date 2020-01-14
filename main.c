#include <alsa/asoundlib.h>
#include <math.h>
#include "sound_engine.h"
#include "music.h"
#include "wave.h"

static const short SHORT_MAX = 0x7fff;
static const float volume = 0.1;

float sin_function(float frequency, float t) {
    return sin(t * frequency * 2*M_PI * 2);
}

float saw_function(float frequency, float t) {
    float thing = t * frequency * 2.0f;
    float val = fmod(thing, 1.0f);
    return val;
}

float square_function(float frequency, float t) {
    int high = (int)((t*2*2) * frequency) % 2 == 0;
    return (high ? 1.0f : -1.0f);
}

int main() {
    if(!init_sound_engine()) {
        fprintf(stderr, "Error initializing sound engine, exiting!\n");
        return -1;
    }

    int num_seconds = 30;
    int bytes_per_frame = 4;
    int size_buffer = num_seconds * rate * bytes_per_frame;
    short* buffer = (short*) malloc(num_seconds * rate * bytes_per_frame);

    float chord[] = {
        NOTE_C4 / 2,
        NOTE_E4 / 2,
        NOTE_G4 / 2,
    };
    int size_chord = sizeof(chord)/sizeof(float);

    float mult = 1.0f;
    for (int frame = 0; frame < rate * num_seconds; frame++) {
        float t = (float) frame / rate;

        buffer[2*frame + 0] = 0;
        buffer[2*frame + 1] = 0;
        mult = 1.0f + 0.1 * sin(t*t * 2*M_PI);
        for (int i = 0; i < size_chord; i++) {
            float value = sin_function(chord[i], t);
            short short_value = (short) (value * SHORT_MAX * volume);
            buffer[2*frame + 0] += short_value;
            buffer[2*frame + 1] += short_value;
        }
    }

    printf("size_buffer: %d\n", size_buffer);
    write_to_wave("output.wav", buffer, size_buffer);
    
    snd_pcm_prepare(pcm_handle);

    int pcm_return = 0;
    int offset = 0;
    while (offset < size_buffer / 2) {
        if (pcm_return > 100)
            offset += pcm_return * 2;

        printf("%d, %d\n", offset, pcm_return);

        snd_pcm_prepare(pcm_handle);
        while((pcm_return = snd_pcm_writei(pcm_handle, &(buffer[offset]) , frames_per_buffer)) < 0) {
            snd_pcm_prepare(pcm_handle);
            printf("Buffer Underrun\n");
        }
    }

    free(buffer);
}
