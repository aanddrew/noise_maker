#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "parser.h"
#include "helpers.h"
#include "../music/music.h"
#include "../containers/Vector.h"

#define size_line 1024
static int line_number = 0;

static Vector* instrument_vec= NULL;
static Instrument* find_instrument(const char* name) {
    for(int i = 0; i < instrument_vec->num; i++) {
        Instrument* current_ins = (Instrument*) Vector_get(instrument_vec, i);
        if (!strcmp(name, current_ins->name)) {
            return current_ins;
        }
    }
    return NULL;
}

static MathTree* create_tree(FILE* file, const char* tree_name) {
    char line[size_line];
    MathTree* returned = NULL;
    int exit = 0;
    while(fgets(line, size_line, file) && !exit) {
        *strchr(line, '\n') = '\0';
        int num_tokens;
        char** tokens = tokenize(line, &num_tokens);
        for(int i = 0; i < num_tokens; i++) {
            if (!strcmp(tokens[i], "return")) {
                i++;
                returned = MathTree_init(tree_name, (const char**) &tokens[i], num_tokens - 1);
            }
            else if (!strcmp(tokens[i], "end")) {
                exit = 1;
                break;
            }
        }

        free(tokens);
    }
    return returned;
}

static Instrument* create_instrument(FILE* file, const char* name) {
    char line[size_line];
    Instrument* returned = Instrument_init(name);
    int exit = 0;
    while(fgets(line, size_line, file) && !exit) {
        *strchr(line, '\n') = '\0';
        int num_tokens;
        char** tokens = tokenize(line, &num_tokens);
        for(int i = 0; i < num_tokens; i++) {
            if (!strcmp(tokens[i], "func")) {
                i++;
                returned->tree = MathTree_lookup(tokens[i]);
            }
            else if (!strcmp(tokens[i], "env")) {
                i++;
            }
            else if (!strcmp(tokens[i], "filt")) {
                i++;
                returned->filter = MathTree_lookup(tokens[i]);
            }
            else if (!strcmp(tokens[i], "end")) {
                exit = 1;
                break;
            }
        }

        free(tokens);
    }
    if (!instrument_vec) {
        instrument_vec = Vector_init();
    }
    Vector_push(instrument_vec, (void*) returned);
    return returned;
}

static Track* create_track(FILE* file) {
    char line[size_line];
    Track* returned = Track_init();
    int exit = 0;
    while(fgets(line, size_line, file) && !exit) {
        *strchr(line, '\n') = '\0';
        int num_tokens;
        char** tokens = tokenize(line, &num_tokens);
        for(int i = 0; i < num_tokens; i++) {
            if (!strcmp(tokens[i], "ins")) {
                i++;
                returned->ins = find_instrument(tokens[i]);
            }
            else if (!strcmp(tokens[i], "note")) {
                i++;
                float note_freq = string_to_note(tokens[i]);
                if (note_freq <= 0) {
                    printf("Error: line %d: note %s not recognized", line_number, tokens[i]);
                }
                i++;
                float note_start = atof(tokens[i]);
                i++;
                float note_length = atof(tokens[i]);
                Track_insert_note(returned, note_freq, note_start, note_length);
            }
            else if (!strcmp(tokens[i], "end")) {
                exit = 1;
                break;
            }
        }
        free(tokens);
    }
    return returned;
}

Vector* Parser_parse_song(const char* song_file_name, int* tempo, float* volume) {
    FILE* song_file = fopen(song_file_name, "r");
    if (song_file == NULL) {
        printf("File %s Not Found!\n", song_file_name);
    }

    Vector* track_vec = Vector_init();
    char line[size_line];
    line_number = 0;

    while(fgets(line, size_line, song_file)) {
        *strchr(line, '\n') = '\0';
        int num_tokens = 0;
        char** tokens = tokenize(line, &num_tokens);

        //identify tokens
        for(int i = 0; i < num_tokens; i++) {
            if(!strcmp(tokens[i], "tempo")) {
                i++;
                *tempo = atoi(tokens[i]);
            }
            else if (!strcmp(tokens[i], "volume")) {
                i++;
                *volume = atof(tokens[i]);
            }
            else if (!strcmp(tokens[i], "track")) {
                i++;
                Track* newtrack = create_track(song_file);
                Vector_push(track_vec, (void*) newtrack);
            }
            else if (!strcmp(tokens[i], "func")) {
                i++;
                create_tree(song_file, tokens[i]);
            }
            else if (!strcmp(tokens[i], "ins")) {
                i++;
                create_instrument(song_file, tokens[i]);
            }
        }

        line_number++;
        free(tokens);
    }

    fclose(song_file);

    return track_vec;
}
