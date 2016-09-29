#include "ease.h"
#include "easing.h"

static float doMath(float domain[2], float range[2], AHEasingFunction easeFn){
    float t = (float)domain[0];
    float d = (float)domain[1];
    float b = (float)range[0];
    float c = (float)range[1] - range[0];
    float p = easeFn(t/d);
    return c * p + b;
}

float linear(float domain[2], float range[2]){
    return doMath(domain, range, LinearInterpolation);
}

float easeOut(float domain[2], float range[2]){
    return doMath(domain, range, QuadraticEaseOut);
}
float easeIn(float domain[2], float range[2]){
    return doMath(domain, range, QuadraticEaseIn);
}
float easeInOut(float domain[2], float range[2]){
    return doMath(domain, range, QuadraticEaseInOut);
}

float easeOutBack(float domain[2], float range[2]){
    return doMath(domain, range, BackEaseOut);
}
float easeInBack(float domain[2], float range[2]){
    return doMath(domain, range, BackEaseIn);
}
float easeInOutBack(float domain[2], float range[2]){
    return doMath(domain, range, BackEaseInOut);
}

float easeOutElastic(float domain[2], float range[2]){
    return doMath(domain, range, ElasticEaseOut);
}
float easeInElastic(float domain[2], float range[2]){
    return doMath(domain, range, ElasticEaseIn);
}
float easeInOutElastic(float domain[2], float range[2]){
    return doMath(domain, range, ElasticEaseInOut);
}

float easeOutBounce(float domain[2], float range[2]){
    return doMath(domain, range, BounceEaseOut);
}
float easeInBounce(float domain[2], float range[2]){
    return doMath(domain, range, BounceEaseIn);
}
float easeInOutBounce(float domain[2], float range[2]){
    return doMath(domain, range, BounceEaseInOut);
}
