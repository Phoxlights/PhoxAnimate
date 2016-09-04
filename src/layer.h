#ifndef LAYER_H
#define LAYER_H

#include "constants.h"
#include "keyframe.h"
#include "bitmap.h"
#include "pixel.h"

typedef struct Layer{
    // width and height of this layer
    int width;
    int height;

    // list of keyframes for this layer. They
    // will be played in the order they appear
    Keyframe * keyframes[MAX_KEYFRAMES];

    // number of keyframes in `keyframes`
    int keyframe_count;

    // index of the currently active keyframe
    int current_keyframe;

    // opacity of this layer
    float opacity;

    // unmodified bitmap, set at the beginning
    // of each keyframe. All keyframe 
    // transformations are made to this bitmap
    Bitmap * pristine_bitmap;

    // the bitmap resulting from the last
    // keyframe and transforms
    Bitmap * last_bitmap;

    // indicates if layer is stopped, bro
    bool stopped;

    // TODO - blend modes
    // TODO - loop count
} Layer;

// creates a shiny new layer with sensible
// default values
Layer * Layer_create(int width, int height);
void Layer_free(Layer * layer);

void Layer_add_keyframe(Layer * layer, Keyframe * keyframe);

// increments a layers current frame and
// automatically updates `current_keyframe`
// if appropriate
// TODO - return value indicating if looped
void Layer_tick(Layer * layer);
void Layer_composite(Layer * layer);

void Layer_stop(Layer * layer);
void Layer_play(Layer * layer);
// TODO - pause

// update animation bitmap. assumes the entire animation
// has only 1 bitmap
int Layer_updateBitmap(Layer * layer, byte * buffer);

#endif
