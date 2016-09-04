#include "progressbar.h"

typedef struct TransformConfig {
    unsigned int * total;
    unsigned int * progress;
} TransformConfig;

static int mirroredUpdate(byte * buffer, int length, unsigned int progress, unsigned int total){
    int halfLength = length / 2;
    float ratio = (float)progress / (float)total;
    float inverseRatio = (float)1 - ratio;
    float p = (float)halfLength * ratio;
    float r = p - (int)p;
    int offsetA, offsetB;

    for(int i = 0; i < halfLength; i++){
        offsetA = i * STRIDE;
        offsetB = (length - 1 - i) * STRIDE;
        if(p >= i){
            if(p - i < 1){
                // partially illluminate this pixel to
                // show the fractional value
                buffer[offsetA] = buffer[offsetB] = 255 * r * inverseRatio;
                buffer[offsetA+1] = buffer[offsetB+1] = 0;
                buffer[offsetA+2] = buffer[offsetB+2] = 255 * r * ratio;
                buffer[offsetA+3] = buffer[offsetB+3] = 255;
            } else {
                buffer[offsetA] = buffer[offsetB] = 255 * inverseRatio;
                buffer[offsetA+1] = buffer[offsetB+1] = 0;
                buffer[offsetA+2] = buffer[offsetB+2] = 255 * ratio;
                buffer[offsetA+3] = buffer[offsetB+3] = 255;
            }
        }
    }
    return 1;
}

static void tick(void * t, Bitmap * bitmap, int domain[2]){
    TransformConfig * transform = (TransformConfig*)t;
    mirroredUpdate(bitmap->data, bitmap->width, *transform->progress, *transform->total);
}

static void freeTransform(void * t){
    TransformConfig * transform = (TransformConfig*)t;
    free(transform);
}

Transform createTransformProgressBar(unsigned int * progress, unsigned int * total){
    TransformConfig * transform = (TransformConfig*)malloc(sizeof(TransformConfig));
    transform->progress = progress;
    transform->total = total;
    // TODO - configurable style
    return createTransform((void*)transform, tick, freeTransform);
}
