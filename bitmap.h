#ifndef BITMAP_H
#define BITMAP_H

#include "constants.h"
#include "blendingmode.h"

#define MAX_STOPS 5

typedef struct Bitmap {
    int width;
    int height;
    byte * data;
} Bitmap;

typedef struct GradientStop{
    byte r;
    byte g;
    byte b;
    byte a;
} GradientStop;

// make bitmap good god almighty
// create empty Bitmap struct
Bitmap * Bitmap_create(int width, int height);
Bitmap * Bitmap_create(int width, int height, byte * bitmap_data);
void Bitmap_free(Bitmap * bitmap);
void Bitmap_copy(Bitmap * target, Bitmap * source);
void Bitmap_copyFromBuffer(Bitmap * target, byte * buffer);
void Bitmap_fill(Bitmap * target, byte rgba[4]);
void Bitmap_gradient(Bitmap * target, GradientStop * stops[MAX_STOPS], int numStops);

void Bitmap_blend(Bitmap * target, Bitmap * source, BlendingMode mode);

void Bitmap_get(Bitmap * bitmap, int x, int y, byte * rgba);
void Bitmap_set(Bitmap * bitmap, int x, int y, byte * rgba);

#endif
