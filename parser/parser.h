#ifndef PARSER_H
#define PARSER_H

#include "../music/Track.h"
#include "../containers/Vector.h"

Vector* Parser_parse_song(const char* song_file_name, int* tempo, float* volume); 

#endif
