#ifndef PARSER_H
#define PARSER_H

#include "../music/Track.h"

Track* Parser_parse(const char* song_file_name, int* num_tracks, int* tempo); 

#endif
