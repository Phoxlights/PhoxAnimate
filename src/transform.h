#ifndef TRANSFORM_H
#define TRANSFORM_H

/*
 * interface for working with transforms without
 * needing to know much about them
 */

#include "bitmap.h"

typedef struct TransformNode TransformNode;

// public handle for passing around transforms
typedef TransformNode* Transform;

// assembles all the info needed to use a transform
// NOTE: this should be used by transforms, not by client
Transform createTransform(void* transform, void (*transformFn)(void*, Bitmap*, int*), void (*freeFn)(void*));

// put a transform to work. this can be used by the client
// or by the transform itself
void applyTransform(Transform transform, Bitmap * bitmap, int domain[2]);
void freeTransform(Transform transform);

#endif
