#ifndef BLENDING_MODE_H
#define BLENDING_MODE_H

#include <Arduino.h>
#include "constants.h"

typedef enum BlendingMode {
    REPLACE,
    ALPHA,
    OVERLAY,
    ADD,
    SUBTRACT,
    MULTIPLY
} BlendingMode;

// NOTE - target and source are pointers to
// an array of 4 bytes, rgba
void blend(byte * target, byte * source, BlendingMode mode);

#endif
