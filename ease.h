#ifndef EASE_H
#define EASE_H

// TODO - more easing functions
typedef enum EaseFunction {
    LINEAR
} EaseFunction;

float linear(float domain[2], float range[2]);

#endif
