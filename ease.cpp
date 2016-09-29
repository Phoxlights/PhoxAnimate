#include "ease.h"
#include "easing.h"

float ease(float domain[2], float range[2], EaseFunction ease){
    AHEasingFunction fn;
    switch(ease){
        case EASE_IN:
            fn = QuadraticEaseIn;
            break;
        case EASE_OUT:
            fn = QuadraticEaseOut;
            break;
        case EASE_IN_OUT:
            fn = QuadraticEaseInOut;
            break;
        case EASE_IN_BACK:
            fn = BackEaseIn;
            break;
        case EASE_OUT_BACK:
            fn = BackEaseOut;
            break;
        case EASE_IN_OUT_BACK:
            fn = BackEaseInOut;
            break;
        case EASE_IN_ELASTIC:
            fn = ElasticEaseIn;
            break;
        case EASE_OUT_ELASTIC:
            fn = ElasticEaseOut;
            break;
        case EASE_IN_OUT_ELASTIC:
            fn = ElasticEaseInOut;
            break;
        case EASE_IN_BOUNCE:
            fn = BounceEaseIn;
            break;
        case EASE_OUT_BOUNCE:
            fn = BounceEaseOut;
            break;
        case EASE_IN_OUT_BOUNCE:
            fn = BounceEaseInOut;
            break;
        case LINEAR:
        default:
            fn = LinearInterpolation;
            break;
    }

    float t = (float)domain[0];
    float d = (float)domain[1];
    float b = (float)range[0];
    float c = (float)range[1] - range[0];
    float p = fn(t/d);
    return c * p + b;
}
