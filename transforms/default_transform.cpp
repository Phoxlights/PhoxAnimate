#include "transform.h"
#include "default_transform.h"
#include "bitmap.h"

typedef struct TransformConfig {
    // range 0-1
    float begin;
    // range 0-1
    float end;
} TransformConfig;

void tickTransformDefault(void * t, Bitmap * bitmap, int domain[2]){
    TransformConfig * transform = (TransformConfig*)t;
    // TODO - do some work with transform and bitmap
}

void freeTransformDefault(void * t){
    TransformConfig * transform = (TransformConfig*)t;
    free(transform);
}

Transform createTransformDefault(float begin, float end){
    TransformConfig * transform = (TransformConfig*)malloc(sizeof(TransformConfig));
    // TODO - do some work with args, default values, etc
    return createTransform((void*)transform, tickTransformDefault, freeTransformDefault);
}
