#include <stdarg.h>
#include "stdlib.h"
#include "constants.h"
#include "keyframe.h"
#include "transform.h"
#include "blendingmode.h"
#include "bitmap.h"

Keyframe * Keyframe_create(int duration){
    Keyframe * keyframe = (Keyframe*)malloc(sizeof(Keyframe));

    keyframe->duration = duration;
    void * bitmap;
    keyframe->bitmap = (Bitmap*)bitmap;

    // TODO - get list of transforms
    keyframe->transform_count = 0;

    Keyframe_init(keyframe);

    return keyframe;
}
Keyframe * Keyframe_create(int duration, Bitmap * bitmap){
    Keyframe * keyframe = (Keyframe*)malloc(sizeof(Keyframe));

    keyframe->duration = duration;
    keyframe->bitmap = bitmap;

    keyframe->transform_count = 0;

    Keyframe_init(keyframe);

    return keyframe;
}

void Keyframe_free(Keyframe * keyframe){
    if(keyframe->transform_count){
        for(int i = 0; i < keyframe->transform_count; i++){
            freeTransform(keyframe->transforms[i]);
        }
    }
    free(keyframe);
}

// reset stuff to initial values so this
// keyframe can be reused
void Keyframe_init(Keyframe * keyframe){
    keyframe->current_frame = 0;
}

void Keyframe_add_transform(Keyframe * keyframe, Transform transform){
    int curr_transform = keyframe->transform_count;

    // cant add anymore transforms
    if(curr_transform == MAX_TRANSFORMS){
        return;
    }

    keyframe->transforms[curr_transform] = transform;
    keyframe->transform_count++;
}

int Keyframe_updateBitmap(Keyframe * keyframe, byte * buffer){
    Bitmap_copyFromBuffer(keyframe->bitmap, buffer);
    return 1;
}
