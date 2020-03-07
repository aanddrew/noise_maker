#ifndef TRACK_H
#define TRACK_H

#include "Pattern.h"
#include "../containers/Vector.h"

typedef struct {
    Vector* patterns;
    Vector* offsets;
} Track;

Track* Track_init();
void Track_delete(Track* track);

void Track_paint(Track* track, float* canvas);
void Track_add_pattern(Track* track, Pattern* pattern);
void Track_insert_offset(Track* track, Pattern* pattern, float offset);

#endif
