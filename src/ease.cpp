#include "ease.h"

float linear(float domain[2], float range[2]){
    float A = (float)domain[0];
    float B = (float)domain[1];
    float C = (float)range[0];
    float D = (float)range[1];

	return (A / B) * (D - C) + C;
}
