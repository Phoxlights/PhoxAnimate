#include "constants.h"
#include "layer.h"
#include "transform.h"
#include "blendingmode.h"
#include "bitmap.h"

Layer * Layer_create(int width, int height){
    Layer * layer = (Layer*)malloc(sizeof(Layer));

    layer->width = width;
    layer->height = height;
    layer->opacity = 1.0;
    layer->pristine_bitmap = Bitmap_create(width, height);
    layer->last_bitmap = Bitmap_create(width, height);
    layer->current_keyframe = 0;
    layer->keyframe_count = 0;
    layer->stopped = false;

    return layer;
}

void Layer_free(Layer * layer){
    Bitmap_free(layer->pristine_bitmap);
    Bitmap_free(layer->last_bitmap);

    // TODO - should this happen here?
    if(layer->keyframe_count){
        for(int i = 0; i < layer->keyframe_count; i++){
            Keyframe_free(layer->keyframes[i]);
        }
    }

    free(layer);
}

// adds keyframe to this layer's list of keyframes
// cmon, keep up guys.
void Layer_add_keyframe(Layer * layer, Keyframe * keyframe){
   int curr_keyframe = layer->keyframe_count;

   // if this is the first keyframe to be added,
   // use its bitmap data as the pristine bitmap
   // TODO - ensure keyframe has bitmap
   // TODO - `tick` does this plus more to setup
   // the keyframe... probably need to use that instead
   if(curr_keyframe == 0){
       Bitmap_copy(layer->pristine_bitmap, keyframe->bitmap);
   }

   // cant add anymore keyframes
   if(curr_keyframe == MAX_KEYFRAMES){
       return;
   }

   layer->keyframes[curr_keyframe] = keyframe;

   layer->keyframe_count++;
}

// increment the current frame for a layer, including
// incrementing the current keyframe if necessary
void Layer_tick(Layer * layer){
    // if we dont have any keyframes, there
    // is no work to be done
    if(layer->keyframe_count == 0){
        return;
    }

    // if this layer is stopped, dont do nothin!
    if(layer->stopped){
        return;
    }

    Keyframe * current = layer->keyframes[layer->current_keyframe];

    // if the keyframe duration is -1, run forever!
    if(current->duration == -1){
        return;
    }

    // if we can simply increment the current
    // keyframe, do it
    if(current->current_frame + 1 <= current->duration){
        current->current_frame += 1;

    // otherwise, switch to a new keyframe
    } else {
        int next_keyframe = (layer->current_keyframe + 1) % layer->keyframe_count;
        layer->current_keyframe = next_keyframe;

        // make sure this guy is super clean
        current = layer->keyframes[layer->current_keyframe];
        Keyframe_init(current);

        // if this keyframe has a bitmap, use it
        if(current->bitmap){
            Bitmap_copy(layer->pristine_bitmap, current->bitmap);
        }

        // set last_bitmap to the pristine bitmap
        // TODO - this might not be necessary
        Bitmap_copy(layer->last_bitmap, layer->pristine_bitmap);
    }
}

void Layer_composite(Layer * layer){
    // if we dont have any keyframes, there
    // is no work to be done
    if(layer->keyframe_count == 0){
        return;
    }

    // if this layer is stopped, dont do nothin!
    if(layer->stopped){
        return;
    }

    Keyframe * current = layer->keyframes[layer->current_keyframe];

    // clean copy of pristine bitmap to operate on 
    Bitmap_copy(layer->last_bitmap, layer->pristine_bitmap);

    // if any transforms are present, apply them
    Transform transform;
    if(current->transform_count){
        for(int i = 0; i < current->transform_count; i++){
            transform = current->transforms[i];
            int domain[] = {current->current_frame, current->duration};
            applyTransform(transform, layer->last_bitmap, domain);
        }
    }

    // apply layer->opacity
    // TODO - use a transform to apply this
    Bitmap * bitmap = layer->last_bitmap;
    int num_pixels = bitmap->width * bitmap->height;
    for(int i = 0; i < num_pixels; i++){
        bitmap->data[i * STRIDE + 3] *= layer->opacity;
    }
}

void Layer_stop(Layer * layer){
    layer->stopped = true;
    layer->current_keyframe = 0;
    // TODO - ensure current keyframe's
    // current frame is reset
    byte nothin[] = {0,0,0,0};
    Bitmap_fill(layer->last_bitmap, nothin);
}
void Layer_play(Layer * layer){
    layer->stopped = false;
}

int Layer_updateBitmap(Layer * layer, byte * buffer){
    // NOTE - assumes keyframe 0 has a bitmap and
    // that is the only bitmap in this animation
    bool wasStopped = layer->stopped;
    Layer_stop(layer);
    Keyframe * current = layer->keyframes[layer->current_keyframe];
    Keyframe_updateBitmap(current, buffer);
    // TODO - maybe less copyin' and stuff here?
    Bitmap_copy(layer->pristine_bitmap, current->bitmap);
    Bitmap_copy(layer->last_bitmap, layer->pristine_bitmap);
    // twerent stopped when we got here, so who am
    // I to say like "hey, you stay down"? right?
    if(!wasStopped){
        Layer_play(layer);
    }
}
