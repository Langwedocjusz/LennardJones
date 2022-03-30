#pragma once

#include <vector>
#include "../Utils.h"

class NaiveCPUSim {
public:
	NaiveCPUSim() = default;
	~NaiveCPUSim() = default;

	NaiveCPUSim(std::vector<Particle>* user_pointer, float dt, 
					float epsilon, float sigma, float l);

	void setDataPointer(std::vector<Particle>* user_pointer) { m_Data = user_pointer; }
	void setDT(float dt) { m_DT = dt; }
	void setEpsilon(float epsilon) { m_Epsilon = epsilon; }
	void setSigma(float sigma) { m_Sigma = sigma; }
	void setL(float l) { m_L = l; }

	void OnUpdate();
private:
	float m_DT = 0.01f, m_Epsilon = 0.1f, m_Sigma = 0.1f, m_L = 1.0f;
	std::vector<Particle>* m_Data = nullptr;
};