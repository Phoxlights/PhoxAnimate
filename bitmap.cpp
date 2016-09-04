#include "constants.h"
#include "bitmap.h"

void Bitmap_fill(Bitmap * target, byte rgba[4]);

Bitmap * Bitmap_create(int width, int height){
    Bitmap * bitmap = (Bitmap*)malloc(sizeof(Bitmap));

    bitmap->width = width;
    bitmap->height = height;
    bitmap->data = (byte*)malloc(width * height * STRIDE);
    // TODO - zero out

    byte rgba[] = {0,0,0,0};

    // fill with nothing man, nothing
    Bitmap_fill(bitmap, rgba);

    return bitmap;
}

Bitmap * Bitmap_create(int width, int height, byte * bitmap_data){

    Bitmap * bitmap = (Bitmap*)malloc(sizeof(Bitmap));

    int bitmapLength = width * height * STRIDE;

    bitmap->width = width;
    bitmap->height = height;
    bitmap->data = (byte*)malloc(bitmapLength);
    // TODO - zero out

    for(int i = 0; i < bitmapLength; i++){
        // TODO - ensure bitmap[i] is valid
        bitmap->data[i] = bitmap_data[i];
    }

    return bitmap;
}

void Bitmap_free(Bitmap * bitmap){
    free(bitmap->data);
    free(bitmap);
}

void Bitmap_copy(Bitmap * target, Bitmap * source){
    // NOTE - for now this assumes target and source are the same size
    int length = target->width * target->height * STRIDE;
    for(int i = 0; i < length; i++){
        // TODO - if i exceeds source bounds, fill with 0
        target->data[i] = source->data[i];
    }
}

void Bitmap_copyFromBuffer(Bitmap * target, byte * buffer){
    // NOTE - for now this assumes target and source are the same size
    int length = target->width * target->height * STRIDE;
    for(int i = 0; i < length; i++){
        // TODO - if i exceeds source bounds, fill with 0
        target->data[i] = buffer[i];
    }
}

void Bitmap_fill(Bitmap * target, byte rgba[4]){
    int num_px = target->width * target->height;

    for(int i = 0; i < num_px; i++){
        int offset = i * STRIDE;
        target->data[offset] = rgba[0];
        target->data[offset+1] = rgba[1];
        target->data[offset+2] = rgba[2];
        target->data[offset+3] = rgba[3];
    }
}

void Bitmap_get(Bitmap * bitmap, int x, int y, byte * rgba){
    int row_offset = bitmap->width * y * STRIDE;
    int col_offset = x * STRIDE;
    int offset = row_offset + col_offset;

    rgba[0] = bitmap->data[offset];
    rgba[1] = bitmap->data[offset+1];
    rgba[2] = bitmap->data[offset+2];
    rgba[3] = bitmap->data[offset+3];
}
void Bitmap_set(Bitmap * bitmap, int x, int y, byte * rgba){
    int row_offset = bitmap->width * y * STRIDE;
    int col_offset = x * STRIDE;
    int offset = row_offset + col_offset;

    bitmap->data[offset] = rgba[0];
    bitmap->data[offset+1] = rgba[1];
    bitmap->data[offset+2] = rgba[2];
    bitmap->data[offset+3] = rgba[3];
}

void Bitmap_blend(Bitmap * target, Bitmap * source, BlendingMode mode) {
    // NOTE - assumes source is same size as target
    int num_px = target->width * target->height;

    for(int i = 0; i < num_px; i++){
        int offset = i * STRIDE;
        blend(target->data + offset, source->data + offset, mode);
    }
}

// compressed pixel data for efficient data transfer
// the layout is as follows:
// 8 bits - x
// 8 bits - y
// 4 bits - red
// 4 bits - green
// 4 bits - blue
// 4 bits - alpha
typedef struct PackedPixels {
    int num_pixels;
    byte * data;
} PackedPixels;

PackedPixels * PackedPixels_create(byte * px_data, int num_pixels);
// takes a PackedPixels struct and returns
// and unpacked Bitmap struct
Bitmap * PackedPixels_unpack();
