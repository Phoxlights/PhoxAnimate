#include <stdlib.h>
#include "transform.h"
#include "fire.h"
#include "bitmap.h"

typedef struct TransformConfig {
    int frequency;
    float * brightness;
    int flickerCount;
    int currFrame;
} TransformConfig;

float randomBrightness(){
    // loosely inspired by https://cpldcpu.wordpress.com/2013/12/08/hacking-a-candleflicker-led/
    int val = rand() % 100;
    // 50% chance for max brightness. also gets rid
    // of overly dim values
    if(val <= 50){
        val = 100;
    }
    return val * 0.01;
}

void tickTransformFire(void * t, Bitmap * bitmap, int domain[2]){
    TransformConfig * transform = (TransformConfig*)t;
    int num_pixels = bitmap->width * bitmap->height;
    int numGroups = num_pixels / transform->flickerCount;

    transform->currFrame++;

    // check if its time to generate a
    // new brightness and duration
    if(transform->currFrame >= transform->frequency){
        for(int i = 0; i < transform->flickerCount; i++){
            transform->brightness[i] = randomBrightness();
        }
        transform->currFrame = 0;
    }
    
    // apply flicker to alpha channel
    for(int i = 0; i < num_pixels; i++){
        int offset = i * STRIDE;
        bitmap->data[offset+3] *= transform->brightness[i / numGroups];
    }
}

void freeTransformFire(void * t){
    TransformConfig * transform = (TransformConfig*)t;
    free(transform->brightness);
    free(transform);
}

Transform createTransformFire(int frequency, int flickerCount){
    TransformConfig * transform = (TransformConfig*)malloc(sizeof(TransformConfig));
    transform->currFrame = 0;
    transform->frequency = frequency;
    transform->flickerCount = flickerCount;
    transform->brightness = (float*)malloc(sizeof(float) * flickerCount);
    for(int i = 0; i < flickerCount; i++){
        transform->brightness[i] = randomBrightness();
    }
    return createTransform((void*)transform, tickTransformFire, freeTransformFire);
}
