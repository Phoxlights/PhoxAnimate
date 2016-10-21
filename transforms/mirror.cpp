#include "mirror.h"
#include <math.h>
#include "bitmap.h"

typedef struct TransformConfig {
    bool clone;
} TransformConfig;

// mirrors in half on the x axis
void tickTransformMirrorX(void * t, Bitmap * bitmap, int domain[2]){
    TransformConfig * transform = (TransformConfig*)t;
    // TODO - first half or second half?

    int num_pixels = bitmap->width * bitmap->height;
    int halfsies = floor(num_pixels / 2);

    for(int i = halfsies; i < num_pixels; i++){
        int offset = i * STRIDE;
        int offsetb;
        if(transform->clone){
            offsetb = (num_pixels - 1 - i) * STRIDE;
        } else {
            offsetb = (i - halfsies) * STRIDE;
        }
        bitmap->data[offset] = bitmap->data[offsetb];
        bitmap->data[offset+1] = bitmap->data[offsetb+1];
        bitmap->data[offset+2] = bitmap->data[offsetb+2];
        bitmap->data[offset+3] = bitmap->data[offsetb+3];
    }
}

void freeTransformMirrorX(void * t){
    TransformConfig * transform = (TransformConfig*)t;
    free(transform);
}

Transform createTransformMirrorX(bool clone){
    TransformConfig * transform = (TransformConfig*)malloc(sizeof(TransformConfig));
    transform->clone = clone;
    return createTransform((void*)transform, tickTransformMirrorX, freeTransformMirrorX);
}
