#include "pulse.h"
#include "transform.h"
#include "alpha.h"

typedef enum PulseState { FADE_IN, HOLD_IN, FADE_OUT, HOLD_OUT };
typedef struct TransformConfig {
    int fadeIn;
    int holdIn;
    int fadeOut;
    int holdOut;
    int current[2];
    PulseState state;
    Transform alphaTransform;
} TransformConfig;

void freeTransformPulse(void * t){
    TransformConfig * transform = (TransformConfig*)t;
    freeTransform(transform->alphaTransform);
    free(transform);
}

void tickTransformPulse(void * t, Bitmap * bitmap, int domain[2]){
    TransformConfig * transform = (TransformConfig*)t;

    // increment frame counter
    transform->current[0]++;

    // check if we should switch to next transform
    // TODO - this is basically replicating the way layers
    // iterate through keyframes
    if(transform->current[0] >= transform->current[1]){

        switch(transform->state){
            case FADE_IN:
                // create hold transform
                freeTransform(transform->alphaTransform);
                transform->alphaTransform = createTransformAlpha(1.0, 1.0);
                transform->state = HOLD_IN;
                transform->current[0] = 0;
                transform->current[1] = transform->holdIn;
                break;

            case HOLD_IN:
                // create fade out transform
                freeTransform(transform->alphaTransform);
                transform->alphaTransform = createTransformAlpha(1.0, 0.0);
                transform->state = FADE_OUT;
                transform->current[0] = 0;
                transform->current[1] = transform->fadeOut;
                break;

            case FADE_OUT:
                // create hold transform
                freeTransform(transform->alphaTransform);
                transform->alphaTransform = createTransformAlpha(0.0, 0.0);
                transform->state = HOLD_OUT;
                transform->current[0] = 0;
                transform->current[1] = transform->holdOut;
                break;

            case HOLD_OUT:
                // create fade in transform
                freeTransform(transform->alphaTransform);
                transform->alphaTransform = createTransformAlpha(0.0, 1.0);
                transform->state = FADE_IN;
                transform->current[0] = 0;
                transform->current[1] = transform->fadeIn;
                break;

            default:
                break;
        }
    }

    // apply the transform
    applyTransform(transform->alphaTransform, bitmap, transform->current);
}

Transform createTransformPulse(int fadeIn, int holdIn, int fadeOut, int holdOut){
    TransformConfig * transform = (TransformConfig*)malloc(sizeof(TransformConfig));
    transform->fadeIn = fadeIn;
    transform->holdIn = holdIn;
    transform->fadeOut = fadeOut;
    transform->holdOut = holdOut;

    transform->current[0] = 0;
    transform->current[1] = fadeIn;
    transform->state = FADE_IN;
    transform->alphaTransform = createTransformAlpha(0.0, 1.0);

    return createTransform((void*)transform, tickTransformPulse, freeTransformPulse);
}
