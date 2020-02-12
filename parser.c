#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "math_parser.h"
#include "parser.h"
#include "music.h"

typedef struct {
    char* name;
    MathNode* tree;
} FunctionTree;

typedef struct {
    FunctionTree* table;
    int num;
    int size;
} FunctionTable;

static FunctionTable function_table;

static const char* sin_expr[]      = {"f", "t", "sin"};
static const char* saw_expr[]      = {"f", "t", "saw"};
static const char* triangle_expr[] = {"f", "t", "triangle"};
static const char* square_expr[]   = {"f", "t", "square"};

static const char** default_expressions[] = {
    sin_expr,
    saw_expr,
    triangle_expr,
    square_expr,
};

static const int num_default_expressions = sizeof(default_expressions)/sizeof(char**);

void init_parser() {
    function_table.table = malloc(sizeof(FunctionTree) * 4);
    function_table.num = 4;
    function_table.size = 4;

    //table starts with the 4 default ones
    for(int i = 0; i < num_default_expressions; i++) {
        MathNode* tree = create_ast(default_expressions[i], 3);

        FunctionTree function_tree = {
            .name = strdup(default_expressions[i][2]),
            .tree = tree
        };

        function_table.table[i] = function_tree;
    }
}

void cleanup_parser() {
    for(int i = 0; i < function_table.num; i++) {
        free(function_table.table[i].name);
    }
}

void create_table_entry(const char* name) {
    if (function_table.num >= function_table.size) {
        function_table.table = realloc(function_table.table, sizeof(FunctionTree) * function_table.size * 2);
        function_table.size *= 2;
    }

    function_table.table[function_table.num].name = strdup(name);
}

void add_tree_to_table(MathNode* tree) {
    function_table.table[function_table.num].tree = tree;
}

MathNode* get_function_tree(const char* name) {
    for(int i = 0; i < function_table.num; i++) {
        if (!strcmp(name, function_table.table[i].name)) {
            return function_table.table[i].tree;
        }
    }
    return NULL;
}

sound_func_t get_sound_func(const char* name) {
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

static void resize_tracks(Track** tracks, int num_tracks, int* size_tracks) {
    if (num_tracks >= *size_tracks) {
        *size_tracks *= 2;
        *tracks = realloc(*tracks, *size_tracks * sizeof(Track));
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
    //char tokens[size_line * 2];
    char* tokens = malloc(sizeof(char) * size_line * 2);
    int* token_starts = malloc(sizeof(char) * size_line);

    while(fgets(line, size_line, song_file)) {
        resize_tracks(&tracks, num_tracks, &size_tracks);
        
        memset(tokens, ' ', size_line * 2);
        memset(token_starts, 0x0, size_line);

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
            }
            else if (!strcmp(current_token, "track")) {
                i++;
                //current_instrument.function = get_sound_func(current_token);
                current_instrument.tree = get_function_tree(current_token);
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
            else if (!strcmp(current_token, "func")) {
                i++;
                create_table_entry(current_token);
            }
            else if (!strcmp(current_token, "return")) {
                //create a tokens array
                const char** temp_tokens = malloc(sizeof(char*) * (c-1));
                for(int j = 1; j < c; j++) {
                    i++;
                    temp_tokens[j-1] = current_token;
                }
                MathNode* tree = create_ast(temp_tokens, c - 1);
                add_tree_to_table(tree);
                free(temp_tokens);
            }
            else if (!strcmp(current_token, "end")) {
                i++;
                if (!strcmp(current_token, "track")) {
                    num_tracks++;
                }
                if (!strcmp(current_token, "func")) {
                    function_table.num++;
                }

            }
        }

        line_number++;
    }

    free(tokens);
    free(token_starts);
    fclose(song_file);

    *num_tracks_out = num_tracks;
    return tracks;
}
