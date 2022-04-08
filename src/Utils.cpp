#include "Utils.h"

#include <cmath>
#include <limits>

Particle::Particle(float x, float y, float v_x, float v_y) 
	: pos_x(x), pos_y(y), vel_x(v_x), vel_y(v_y)
{}

void utils::Kahan_summation(float& sum, float input, float& c) {
    float y = input - c;
    float t = sum + y;
    c = (t - sum) - y;
    sum = t;
}

float utils::safediv(float x, float y) {
    float div = x / y;

    if (isnan(div) || isinf(div))
        return 0.0f;
    else
        return div;
}