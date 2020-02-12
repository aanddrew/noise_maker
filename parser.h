#include "music.h"
#include "math_parser.h"
#include "types.h"

Track* parse_song(const char* song_file_name, int* num_tracks, int* tempo); 

void init_parser();
MathNode* get_function_tree(const char* name);
sound_func_t get_sound_func(const char* name);
