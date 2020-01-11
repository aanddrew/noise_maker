#include <alsa/asoundlib.h>
#include <math.h>
#include "sound_engine.h"
#include "music.h"
#include "wave.h"

static const short SHORT_MAX = 0x7fff;

//generates a 16 bit integer sin wave based on the sound engine rate
void generate_sin_wave(float frequency, Sample* sample) {
    int num_frames_per_sin_period = (int) ((float) rate/frequency);
    sample->period = num_frames_per_sin_period * 4;

    //we write 32 bits at a time
    for(int i = 0; i < sample->length / 4; i++) {
        float t = ( ((float) (i % num_frames_per_sin_period)) / num_frames_per_sin_period) * 2 * M_PI;
        short value = (short) (sin(t) * SHORT_MAX) / 8;

        sample->buffer[i*2+0] = value;
        sample->buffer[i*2+1] = value;
    }
}

void generate_saw_wave(float frequency, short * data, int length) {
    /*
    for(int i = 0; i < frames_per_buffer; i++) {
        short s1 = (i % 512) * 100 - 5000;  
        short s2 = (i % 1024) * 100 - 5000;  
        s1 /= 16;
        s2 /= 16;
        buffer[4*i+0] = (unsigned char)s1;
        buffer[4*i+1] = s1 >> 8;
        buffer[4*i+2] = (unsigned char)s2;
        buffer[4*i+3] = s2 >> 8;
    }
    */
}

//let's figure out some kind of modular copy method

int main() {
    if(!init_sound_engine()) {
        fprintf(stderr, "Error initializing sound engine, exiting!\n");
        return -1;
    }

    Sample sample;
    sample.length = frames_per_buffer * 4 * 2;
    sample.buffer = (short*) malloc(sample.length);
    generate_sin_wave(NOTE_C4 * 2, &sample);

    int song_length = frames_per_buffer * 64;
    short* song = (short*) malloc(song_length);

    printf("song_length: %d\n", song_length);
    printf("sample.period: %d\n", sample.period);
    /*
    for(int i = 0; i < song_length / sample.period ; i++) {
        memcpy(song + (i * sample.period) / 2 , sample.buffer, sample.period);
    }

    int playhead = 0;
    int pcmreturn;
    for( ; playhead < song_length; playhead += frames_per_buffer * 4) {
        snd_pcm_prepare(pcm_handle);
        while((pcmreturn = snd_pcm_writei(pcm_handle, song + playhead, frames_per_buffer)) < 0) {
            snd_pcm_prepare(pcm_handle);
            fprintf(stderr, "<<<<<<<<<<<<<<< Buffer Underrun >>>>>>>>>>>>>>>\n");
        }
    }
    */

    int num_seconds = 10;
    int bytes_per_frame = 4;
    int size_buffer = num_seconds * rate * bytes_per_frame;
    short* buffer = (short*) malloc(num_seconds * rate * bytes_per_frame);

    float frequency = NOTE_A4;
    for (int frame = 0; frame < rate * num_seconds; frame++) {
        float t = (float) frame / rate;
        float value = 0.2 * sin(t * frequency * 2*M_PI);
        short short_value = (short) (value * SHORT_MAX);

        buffer[2*frame + 0] = short_value;
        buffer[2*frame + 1] = short_value;
    }

    write_to_wave("output.wav", buffer, size_buffer);
    
    /*
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
    */

    free(buffer);
    free(sample.buffer);
    //free(song);
}
