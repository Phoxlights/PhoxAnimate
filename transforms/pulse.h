#ifndef PULSE_H
#define PULSE_H

typedef struct TransformNode TransformNode;
typedef TransformNode* Transform;

Transform createTransformPulse(int fadeIn, int holdIn, int fadeOut, int holdOut);

#endif
