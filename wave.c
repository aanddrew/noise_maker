#include <stdio.h>
#include "../globals.h"

void write_to_wave(const char* file_name, void* buffer, int length) {
    FILE* output = fopen(file_name, "wb");

    //sub chunk 0
    fwrite("RIFF", 4, 1, output);
    int size = 44 + length;
    fwrite(&size,  4, 1, output);
    fwrite("WAVE", 4, 1, output);

    //sub chunk 1
    fwrite("fmt ", 4, 1, output);
    int format_length = 16;
    fwrite(&format_length, 4, 1, output);
    short audio_format = 1;
    fwrite(&audio_format, 2, 1, output);
    short num_channels = 2;
    fwrite(&num_channels, 2, 1, output);

    int temp_wave_rate = WAVE_RATE;
    fwrite(&temp_wave_rate, 4, 1, output);
    int byte_rate = WAVE_RATE * 2 * 2;
    fwrite(&byte_rate, 4, 1, output);
    short block_align = 4;
    fwrite(&block_align, 2, 1, output);
    short bits_per_sample = 16;
    fwrite(&bits_per_sample, 2, 1, output);

    fwrite("data", 4, 1, output);
    //data subchunk
    fwrite(buffer, length, 1, output);

    fclose(output);
}
