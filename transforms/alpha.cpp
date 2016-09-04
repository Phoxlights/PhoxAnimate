#include "transform.h"
#include "alpha.h"
#include "ease.h"
#include "bitmap.h"

typedef struct TransformConfig {
    EaseFunction ease;
    // range 0-1
    float begin;
    // range 0-1
    float end;
} TransformConfig;

void tickTransformAlpha(void * t, Bitmap * bitmap, int domain[2]){
    TransformConfig * transform = (TransformConfig*)t;
    int num_pixels = bitmap->width * bitmap->height;
    float range[] = {transform->begin, transform->end};
    float domain_float[] = {(float)domain[0], (float)domain[1]};
    float multi = linear(domain_float, range);

    for(int i = 0; i < num_pixels; i++){
        int offset = i * STRIDE;
        bitmap->data[offset+3] *= multi;
    }
}

void freeTransformAlpha(void * t){
    TransformConfig * transform = (TransformConfig*)t;
    free(transform);
}

Transform createTransformAlpha(float begin, float end){
    TransformConfig * transform = (TransformConfig*)malloc(sizeof(TransformConfig));
    transform->ease = LINEAR;
    transform->begin = begin;
    transform->end = end;
    return createTransform((void*)transform, tickTransformAlpha, freeTransformAlpha);
}
