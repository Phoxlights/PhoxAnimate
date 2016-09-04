#include "transform.h"
#include "translatex.h"
#include "ease.h"
#include "bitmap.h"

typedef struct TransformConfig {
    EaseFunction ease;
    int begin;
    int end;
    bool wrap;
} TransformConfig;

void tickTransformTranslateX(void * t, Bitmap * bitmap, int domain[2]){
    TransformConfig * transform = (TransformConfig*)t;
    // clone the original / clean bitmap
    Bitmap * original = Bitmap_create(bitmap->width, bitmap->height);
    Bitmap_copy(original, bitmap);

    // start with an empty bitmap
    byte black[] = {0,0,0,0};
    Bitmap_fill(bitmap, black);

    // get the numbers from the magic number box
    float range[] = {(float)transform->begin, (float)transform->end};
    float domain_float[] = {(float)domain[0], (float)domain[1]};
    int offset = floor(linear(domain_float, range));

    byte rgba[4];
    int new_x;

    for(int x = 0; x < bitmap->width; x++){
        for(int y = 0; y < bitmap->height; y++){
            new_x = (int)(x + offset);

            // if new x is out of bounds
            if(new_x >= bitmap->width){
                // and wrapping is off, we're done
                if(!transform->wrap){
                    continue;
                }
                // otherwise, wrap
                new_x = new_x % bitmap->width;
            }

            Bitmap_get(original, x, y, rgba);
            Bitmap_set(bitmap, new_x, y, rgba);
        }
    }

    Bitmap_free(original);
}

void freeTransformTranslateX(void * t){
    TransformConfig * transform = (TransformConfig*)t;
    free(transform);
}

Transform createTransformTranslateX(int begin, int end, bool wrap){
    TransformConfig * transform = (TransformConfig*)malloc(sizeof(TransformConfig));
    transform->ease = LINEAR;
    transform->begin = begin;
    transform->end = end;
    transform->wrap = wrap;
    return createTransform((void*)transform, tickTransformTranslateX, freeTransformTranslateX);
}
