#include "GLCompute.h"

#include "glad/glad.h"

GLComputeSim::GLComputeSim(const std::string& filepath) 
	: m_ComputeShader(filepath)
{}

void GLComputeSim::OnUpdate() {
	m_ComputeShader.Bind();

	m_ComputeShader.setUniform1f("uDT", m_DT);
	m_ComputeShader.setUniform1f("uEpsilon", m_Epsilon);
	m_ComputeShader.setUniform1f("uSigma", m_Sigma);
	m_ComputeShader.setUniform1f("uL", m_L);
	m_ComputeShader.setUniform1f("uInvMass", m_InvMass);
	m_ComputeShader.setUniform1ui("uFirst", int(m_FirstIteration));
	m_ComputeShader.setUniform1ui("uCount", m_NumberOfParticles);

	glBindVertexArray(m_VAO);
	glMemoryBarrier(GL_VERTEX_ATTRIB_ARRAY_BARRIER_BIT | GL_SHADER_STORAGE_BUFFER);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, m_ParticleVBO);
	glDispatchCompute(2 * m_NumberOfParticles / 1024, 1, 1);
}