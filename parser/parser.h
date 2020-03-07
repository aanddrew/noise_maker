#ifndef PARSER_H
#define PARSER_H

#include "../music/Pattern.h"
#include "../containers/Vector.h"

Vector* Parser_parse_song(const char* song_file_name, int* tempo, float* volume); 
void Parser_cleanup();

#endif
