#ifndef KEYFRAME_H
#define KEYFRAME_H

#include "constants.h"
#include "bitmap.h"
#include "transform.h"
#include "blendingmode.h"

typedef struct Keyframe{
    // number of frames this keyframe
    // should last
    int duration;

    // frame this keyframe is on
    int current_frame;

    // optional bitmap for this keyframe
    Bitmap * bitmap;

    // list of transforms to apply to
    // be applied to layer's `pristine_bitmap
    Transform transforms[MAX_TRANSFORMS];

    // number of transforms in `transforms`
    int transform_count;

} Keyframe;

void Keyframe_init(Keyframe * keyframe);
Keyframe * Keyframe_create(int duration);
Keyframe * Keyframe_create(int duration, Bitmap * bitmap);

void Keyframe_free(Keyframe * keyframe);

void Keyframe_add_transform(Keyframe * keyframe, Transform transform);

int Keyframe_updateBitmap(Keyframe * keyframe, byte * buffer);

#endif
