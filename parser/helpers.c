#include "helpers.h"

#include "../music/music.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

char** tokenize(char* line, const char* sep, int* num_ret) {
    int num = 0;
    int size = 4;
    char** returned = malloc(sizeof(char*) * size);
    char* token = strtok(line, sep);

    while(token != NULL) {
        if (num == size) {
            size *= 2;
            returned = realloc(returned, sizeof(char*) * size);
        }
        returned[num] = token;
        token = strtok(NULL, sep);
        num++;
    }

    *num_ret = num;
    return returned;

}

float string_to_note(char* name) {
    int at = 0;

    char letter = tolower(name[at]);
    at++;
    
    float ref_freq = -1.0f;
    int sharp = name[at] == '#';
    if (sharp) at++;

    switch(letter) {
        case 'c': ref_freq = SCALE_4[C + sharp]; break;
        case 'd': ref_freq = SCALE_4[D + sharp]; break;
        case 'e': ref_freq = SCALE_4[E + sharp]; break;
        case 'f': ref_freq = SCALE_4[F + sharp]; break;
        case 'g': ref_freq = SCALE_4[G + sharp]; break;
        case 'a': ref_freq = SCALE_4[A + sharp]; break;
        case 'b': ref_freq = SCALE_4[B + sharp]; break;
    }

    int octave;
    if (!sscanf(&name[at], "%d", &octave)) {
        return -1.0f;
    }

    float factor = pow(2.0f, (octave - 4));

    return ref_freq * factor;
}

