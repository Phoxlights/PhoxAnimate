#include <math.h>

#include "blendingmode.h"

byte clamp(int val){
    //return val > 255 ? 255 : val < 0 ? 0 : val;
    if(val >= 255){
        return 255;
    } else if(val <= 0){
        return 0;
    } else {
        return val;
    }
}

void replace(byte * target, byte * source){
    target[0] = source[0];
    target[1] = source[1];
    target[2] = source[2];
    target[3] = source[3];
}

void alpha(byte * target, byte * source){
    // map 0-255 alpha to 0-1
    // TODO - if alpha == 255, use replace
    float alpha = source[3] / 255.0;
    float alpha_inv = 1 - alpha;

    int r = floor((target[0] * alpha_inv) + (source[0] * alpha));
    int g = floor((target[1] * alpha_inv) + (source[1] * alpha));
    int b = floor((target[2] * alpha_inv) + (source[2] * alpha));
    int a = floor((target[3] * alpha_inv) + source[3]);

    // TODO - is clamp needed?
    target[0] = clamp(r);
    target[1] = clamp(g);
    target[2] = clamp(b);
    target[3] = clamp(a);
}

void blend(byte * target, byte * source, BlendingMode mode){
    switch(mode){
        case ALPHA:
            alpha(target, source);
            break;

        case REPLACE:
            replace(target, source);
            break;
    }
}
