#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "parser.h"
#include "music.h"

static sound_func_t get_sound_func(char* name) {
    if (!strcmp(name, "sin")) {
        return sin_function;
    }
    else if (!strcmp(name, "saw")) {
        return saw_function;
    }
    else if (!strcmp(name, "triangle")) {
        return triangle_function;
    }
    else if (!strcmp(name, "square")) {
        return square_function;
    }
    else {
        return NULL;
    }
}

static float string_to_note(char* name) {
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

static void resize_tracks(Track* tracks, int num_tracks, int* size_tracks) {
    if (num_tracks >= *size_tracks) {
        *size_tracks *= 2;
        tracks = realloc(tracks, *size_tracks * sizeof(Track));
    }
}

Track* parse_song(const char* song_file_name, int* num_tracks_out, int* tempo) {
    FILE* song_file = fopen(song_file_name, "r");
    if (song_file == NULL) {
        printf("File %s Not Found!\n", song_file_name);
    }
    
    Track* tracks = malloc(sizeof(Track) * 4);
    int num_tracks = 0;
    int size_tracks = 4;
    #define current_track tracks[num_tracks]
    
    const size_t size_line = 80;
    char line[size_line];

    int line_number = 0;

    Instrument current_instrument;
    current_instrument.env = default_env;
    current_instrument.filt = default_filt;

    while(fgets(line, size_line, song_file)) {
        resize_tracks(tracks, num_tracks, &size_tracks);

        char tokens[size_line * 2];
        int token_starts[size_line];
        for(int i = 0; i < size_line; i++) {
            token_starts[i] = -1;
        }
        //parse the line into tokens
        int c = 0;
        int start_whitespace = 1;
        for(int i = 0, s = 0; i < size_line; i++) {
            if(start_whitespace && (line[i] == '\t' || line[i] == ' ')) {
                s=i + 1;
                continue;
            }
            else if(line[i] == ' ' || line[i] == '\n'){
                tokens[i] = '\0';
                token_starts[c] = s;
                s = i + 1;
                c++;
                if(line[i] == '\n') break;
            }
            else{
                tokens[i] = line[i];
            }
            start_whitespace = 0;
        }

        //identify tokens
        for(int i = 0; i < c; i++) {
            #define current_token &tokens[token_starts[i]]
            //set tempo
            if(!strcmp(current_token, "tempo")) {
                i++;
                *tempo = atoi(current_token);
                printf("setting tempo to %d\n", *tempo);
            }
            else if (!strcmp(current_token, "track")) {
                i++;
                current_instrument.function = get_sound_func(current_token);
                current_track.ins = current_instrument;
                init_track(&current_track);
            }
            else if (!strcmp(current_token, "note")) {
                i++;
                //if (i >= c) error
                float note_freq = string_to_note(current_token);
                if (note_freq <= 0) {
                    printf("Error: line %d: note %s not recognized", line_number, current_token);
                }
                i++;
                float note_start = atof(current_token);
                i++;
                float note_length = atof(current_token);

                insert_note_into_track(&current_track, note_freq, note_start, note_length);
            }
            else if (!strcmp(current_token, "end")) {
                i++;
                if (!strcmp(current_token, "track")) {
                    num_tracks++;
                }
            }
        }

        line_number++;
    }

    fclose(song_file);

    *num_tracks_out = num_tracks;
    printf("num_tracks: %d\n", num_tracks);
    return tracks;
}
