#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "parser.h"
#include "helpers.h"
#include "../music/music.h"

#define size_line 1024

static MathTree* create_tree(FILE* file, const char* tree_name) {
    char line[size_line];
    MathTree* returned = NULL;
    while(fgets(line, size_line, file)) {
        int num_tokens;
        char** tokens = tokenize(line, &num_tokens);
        for(int i = 0; i < num_tokens; i++) {
            if (!strcmp(tokens[i], "return")) {
                i++;
                returned = MathTree_init(tree_name, (const char**) &tokens[i], num_tokens - 1);
            }
            else if (!strcmp(tokens[i], "end")) {
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
    while(fgets(line, size_line, file)) {
        int num_tokens;
        char** tokens = tokenize(line, &num_tokens);
        for(int i = 0; i < num_tokens; i++) {
            if (!strcmp(tokens[i], "func")) {
                i++;
                returned->tree = MathTree_lookup(tokens[i]);
            }
            else if (!strcmp(tokens[i], "env")) {
                i++;
                returned->tree = MathTree_lookup(tokens[i]);
            }
            else if (!strcmp(tokens[i], "end")) {
                break;
            }
        }

        free(tokens);
    }
    return returned;
}

static Track* create_track(FILE* file) {
    return NULL;
}

Track* Parser_parse(const char* song_file_name, int* num_tracks_out, int* tempo) {
    FILE* song_file = fopen(song_file_name, "r");
    if (song_file == NULL) {
        printf("File %s Not Found!\n", song_file_name);
    }
    
    Track* tracks = malloc(sizeof(Track) * 4);
    int num_tracks = 0;
    int size_tracks = 4;
    #define current_track tracks[num_tracks]

    MathTree* current_tree;
    
    char line[size_line];
    Instrument current_instrument;

    int line_number = 0;
    while(fgets(line, size_line, song_file)) {
        int num_tokens;
        char** tokens = tokenize(line, &num_tokens);

        //identify tokens
        for(int i = 0; i < num_tokens; i++) {
            if(!strcmp(tokens[i], "tempo")) {
                i++;
                *tempo = atoi(tokens[i]);
            }
            else if (!strcmp(tokens[i], "track")) {
                i++;
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

            }
            else if (!strcmp(tokens[i], "func")) {
                i++;
                MathTree* tree = create_tree(song_file, tokens[i]);
                float res = MathNode_evaluate(tree->head, 0,0);
                printf("%f\n", res);
            }
        }

        line_number++;
        free(tokens);
    }

    fclose(song_file);

    *num_tracks_out = num_tracks;
    return tracks;
}
