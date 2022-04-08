#pragma once

struct Particle {
public:
	Particle(float x, float y, float v_x, float v_y);

	float pos_x;
	float pos_y;
	float vel_x;
	float vel_y;
};

namespace utils {
	void Kahan_summation(float& sum, float input, float& c);
	float safediv(float x, float y);
}