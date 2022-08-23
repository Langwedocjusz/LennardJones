#pragma once

#include "../Shader.h"

class GLComputeSim {
public:
	GLComputeSim() = default;
	~GLComputeSim() = default;

	GLComputeSim(const std::string& filepath);

	void setNumber(unsigned int n) { m_NumberOfParticles = n; }
	void setHandles(std::pair<unsigned int, unsigned int> handles) { m_VAO = handles.first; m_ParticleVBO = handles.second; }

	void setDT(float dt) { m_DT = dt; }
	void setEpsilon(float epsilon) { m_Epsilon = epsilon; }
	void setSigma(float sigma) { m_Sigma = sigma; }
	void setL(float l) { m_L = l; }
	void setMass(float m) { m_InvMass = 1 / m; }

	void OnUpdate();
private:
	unsigned int m_VAO, m_ParticleVBO, m_NumberOfParticles;

	Shader m_ComputeShader;

	float m_DT = 0.01f, m_Epsilon = 0.1f, m_Sigma = 0.1f;
	float m_InvMass = 1.0f, m_L = 1.0f, m_InvL = 1.0f;

	bool m_FirstIteration = true;
};