#include "constants.h"
#include "bitmap.h"
#include <ease.h>
#include <math.h>

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

// TODO - rotation (for 2D)
// TODO - probably doesnt work at all in 2D
void Bitmap_gradient(Bitmap * target, GradientStop * stops[MAX_STOPS], int numStops){
    int numPx = target->width * target->height;
    int pxPerStop = ceil(numPx / (numStops-1));

    for(int j = 0; j < numStops-1; j++){
        GradientStop * stopA = stops[j];
        GradientStop * stopB = stops[j+1];
        int offsetStart = j * pxPerStop;
        float domain[2] = {0.0, (float)pxPerStop};
        float rangeR[2] = {(float)stopA->r, (float)stopB->r};
        float rangeG[2] = {(float)stopA->g, (float)stopB->g};
        float rangeB[2] = {(float)stopA->b, (float)stopB->b};
        float rangeA[2] = {(float)stopA->a, (float)stopB->a};

        for(int i = 0; i <= pxPerStop-1; i++){
            int offset = (i + offsetStart) * STRIDE;
            domain[0] = (float)i;
            target->data[offset] = ease(domain, rangeR, LINEAR);
            target->data[offset+1] = ease(domain, rangeG, LINEAR);
            target->data[offset+2] = ease(domain, rangeB, LINEAR);
            target->data[offset+3] = ease(domain, rangeA, LINEAR);
        }

    }

    // smudge last pixel(s) if needed, like a boss?
    // NOTE - assumes just one pixel leftover! any more
    // than one and we need a better smudgin' strategy
    if(numPx % pxPerStop){
        int offset = (numPx-1) * STRIDE;
        GradientStop * lastStop = stops[numStops-1];
        target->data[offset] = lastStop->r;
        target->data[offset+1] = lastStop->g;
        target->data[offset+2] = lastStop->b;
        target->data[offset+3] = lastStop->a;
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
