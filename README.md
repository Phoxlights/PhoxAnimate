Animate
==============
The animate library is a configurable and extendable way to create animations for RGB pixels. [TODO - flesh out descriptions]

Layer
--------
A layer is primarily used to keep track of keyframes. Layers can also be composited together producing a single array of RGB pixels.

Keyframe
--------
A keyframe contains a list of transforms to apply to a bitmap.

Transform
-------
Transforms are functions that take a bitmap and some configuration info, and do some work on that bitmap. Here is a boilerplate transform to use as reference:

    // myTransform.h
    #ifndef DEFAULT_TRANSFORM_H
    #define DEFAULT_TRANSFORM_H

    #include "transform.h"

    Transform createTransformDefault(float begin, float end);

    #endif

and the c file:

    // myTransform.c

    #include "transform.h"
    #include "default_transform.h"
    #include "bitmap.h"

    typedef struct TransformConfig {
        EaseFunction ease;
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

Real good codes
---------------
Easing functions from https://github.com/warrenm/AHEasing
