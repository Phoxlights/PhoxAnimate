#include "transform.h"
#include "rgb.h"
#include "ease.h"
#include "bitmap.h"

typedef struct TransformConfig {
    EaseFunction ease;
    byte begin[3];
    byte end[3];
    BlendingMode mode;
} TransformConfig;

void tickTransformRGB(void * t, Bitmap * bitmap, int domain[2]){
    TransformConfig * transform = (TransformConfig*)t;
    int num_pixels = bitmap->width * bitmap->height;

    float rangeR[] = {(float)transform->begin[0], (float)transform->end[0]};
    float rangeG[] = {(float)transform->begin[1], (float)transform->end[1]};
    float rangeB[] = {(float)transform->begin[2], (float)transform->end[2]};

    float domain_float[] = {(float)domain[0], (float)domain[1]};

    float r = linear(domain_float, rangeR);
    float g = linear(domain_float, rangeG);
    float b = linear(domain_float, rangeB);

    for(int i = 0; i < num_pixels; i++){
        int offset = i * STRIDE;
        // TODO - use mode to determine math to do all up in here
        bitmap->data[offset] = r;
        bitmap->data[offset+1] = g;
        bitmap->data[offset+2] = b;
    }
}

void freeTransformRGB(void * t){
    TransformConfig * transform = (TransformConfig*)t;
    free(transform);
}

Transform createTransformRGB(byte begin[3], byte end[3], BlendingMode mode){
    TransformConfig * transform = (TransformConfig*)malloc(sizeof(TransformConfig));
    transform->ease = LINEAR;
    memcpy(transform->begin, begin, 3);
    memcpy(transform->end, end, 3);
    transform->mode = mode;
    return createTransform((void*)transform, tickTransformRGB, freeTransformRGB);
}
