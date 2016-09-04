#include "transform.h"
#include "twinkle.h"
#include "ease.h"
#include "bitmap.h"

#define MAX_TWINKLES 10

typedef struct TransformConfig {
    // max number of twinkles at one time
    // NOTE - count should be less than MAX_TWINKLES
    int count;
    // how long till a twinkle should be removed
    int duration;
    // variability applied to duration, 1 - 100
    int duration_flux;
    // how dim to make the pixel, 1 - 100
    int strength;

    // calculated
    float strength_float;
    float duration_flux_float;

    // private, for state
    // duration count, x, y
    int twinkles[MAX_TWINKLES][3];

} TransformConfig;

// temporarily dims a randomly selected pixel
void tickTransformTwinkle(void * t, Bitmap * bitmap, int domain[2]){
    TransformConfig * transform = (TransformConfig*)t;
    int new_count;
    int * current_twinkle;
    byte rgba[4];
    for(int i = 0; i < transform->count; i++){
        current_twinkle = transform->twinkles[i];
        new_count = current_twinkle[0];

        // if this twinkle is done/empty
        if(new_count <= 0){
            // add a bit of randomness to the duration
            int flux = (int)random(0, transform->duration) * transform->duration_flux;
            if(random(0,1) == 0){
                flux = -flux;
            }
            // set twinkle duration counter
            current_twinkle[0] = transform->duration + flux;
            // random x location
            current_twinkle[1] = random(0, bitmap->width);
            // random y location
            current_twinkle[2] = random(0, bitmap->height);

        // otherwise this twinkle is active,
        // so use it
        } else {
            //Serial.println("applying a twinkle");
            // apply this twinkle to the image
            int x = current_twinkle[1];
            int y = current_twinkle[2];
            Bitmap_get(bitmap, x, y, rgba);
            rgba[0] *= transform->strength_float;
            rgba[1] *= transform->strength_float;
            rgba[2] *= transform->strength_float;
            Bitmap_set(bitmap, x, y, rgba);

            // countdown twinkle counter
            current_twinkle[0] = new_count - 1;
        }
    }
}

void freeTransformTwinkle(void * t){
    TransformConfig * transform = (TransformConfig*)t;
    free(transform);
}

Transform createTransformTwinkle(int count, int duration, int duration_flux, int strength){
    TransformConfig * transform = (TransformConfig*)malloc(sizeof(TransformConfig));
    // TODO - ensure count is less than MAX_TWINKLES
    transform->count = count;
    transform->duration = duration;

    // NOTE - duration_flux_float should be used, not strength
    transform->duration_flux = duration_flux;
    transform->duration_flux_float = duration_flux * (float)0.01;

    // NOTE - strength_float should be used, not strength
    transform->strength = strength;
    transform->strength_float = 1 - (strength * (float)0.01);

    // store some state on this guy
    // NOTE - MAX_TWINKLES is in constants.h, which
    // is probably the wrong place
    for(int i = 0; i < MAX_TWINKLES; i++){
        // count, x, y
        transform->twinkles[i][0] = 0;
        transform->twinkles[i][1] = 0;
        transform->twinkles[i][2] = 0;
    }
    return createTransform((void*)transform, tickTransformTwinkle, freeTransformTwinkle);
}

