#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "wave.h"
#include "globals.h"
#include "parser/parser.h"
#include "music/music.h"

int main(int argc, char** argv) {

    const char* output_file_name = "output.wav";
    const char* input_file_name = "example_inputs/func.nm";
    if (argc == 1) {
        printf("No File Provided - usage: noise_maker file.nm\n");
        //input_file_name = argv[1];
        //exit(1);
    }
    else {
        input_file_name = argv[1];
        printf("Compiling %s\n", input_file_name);
    }

    for(int i = 1; i < argc; i++) {
        if (!strcmp(argv[i], "-o")) {
            i++;
            if (i >= argc) {
                printf("-o missing output file name, exiting...\n");
                exit(1);
            }
            output_file_name = argv[i];
        }
    }

    //buffer creation
    int num_seconds = 30;
    int size_buffer = num_seconds * WAVE_RATE * 2;
    float* buffer = (float*) malloc(size_buffer * sizeof(float));
    memset(buffer, 0.0f, size_buffer * sizeof(float));
    short* short_buffer = (short*) malloc(size_buffer * sizeof(short));

    //track creation
    int tempo;
    float volume;
    Vector* tracks = Parser_parse_song(input_file_name, &tempo, &volume);
    printf("Song successfully compiled with %d tracks at %d bpm, with master volume %f\n", tracks->num, tempo, volume);

    for(int i = 0; i < tracks->num; i++) {
        Track_paint(Vector_get(tracks, i), buffer);
    }

    master(buffer, size_buffer, volume);
    discretize(short_buffer, buffer, size_buffer);
    printf("Writing song to %s\n", output_file_name);
    write_to_wave(output_file_name, short_buffer, size_buffer);

    free(buffer);
    free(short_buffer);

    for(int i = 0; i < tracks->num; i++) {
        Track_delete(Vector_get(tracks, i));
    }
    Vector_delete(tracks);
}
