#include "ease.h"
#include "transform.h"
#include "bitmap.h"

// generic interface to working with a transform
typedef struct TransformNode {
    void * transform;
    void (*transformFn)(void*, Bitmap*, int*);
    void (*freeFn)(void*);
} TransformNode;

// creates a Transform object, which contains all
// the stuff needed to use a transform, like fo real
Transform createTransform(void* transform, void (*transformFn)(void*, Bitmap*, int*), void (*freeFn)(void*)){
    TransformNode * transformNode = (TransformNode*)malloc(sizeof(TransformNode));
    transformNode->transform = transform;
    transformNode->transformFn = transformFn;
    transformNode->freeFn = freeFn;
    return transformNode;
}

void freeTransform(Transform transform){
    // call the concrete transform's free function
    transform->freeFn(transform->transform);
    free(transform);
}

// apply the given transform to the given bitmap
void applyTransform(Transform node, Bitmap * bitmap, int domain[2]){
    node->transformFn(node->transform, bitmap, domain);
}
