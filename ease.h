#ifndef EASE_H
#define EASE_H

// TODO - more easing functions
typedef enum EaseFunction {
    LINEAR,
    EASE_OUT
} EaseFunction;

float linear(float domain[2], float range[2]);

float easeIn(float domain[2], float range[2]);
float easeOut(float domain[2], float range[2]);
float easeInOut(float domain[2], float range[2]);

float easeInBack(float domain[2], float range[2]);
float easeOutBack(float domain[2], float range[2]);
float easeInOutBack(float domain[2], float range[2]);

float easeInElastic(float domain[2], float range[2]);
float easeOutElastic(float domain[2], float range[2]);
float easeInOutElastic(float domain[2], float range[2]);

float easeInBounce(float domain[2], float range[2]);
float easeOutBounce(float domain[2], float range[2]);
float easeInOutBounce(float domain[2], float range[2]);

#endif
