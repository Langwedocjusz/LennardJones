#pragma once

#include "Utils.h"
#include "Shader.h"

#include "GLFW/glfw3.h"

#include <string>

struct WindowData {
	std::string Title = "Lennard Jones";
	unsigned int Width = 800, Height = 600;
};

class Renderer {
public:
	Renderer(std::vector<Particle>* data);
	Renderer(std::vector<Particle>* data, float l);
	~Renderer();

	void OnRender();
	bool ShouldClose();

	void setSmoothness(float s) { m_pxSmoothness = s; }
	void setQuadSize(float s) { m_quadSize = s; }
	void setBoundarySize(float s) { m_L = s; }
	void UpdateFromCPU(std::vector<Particle>* user_data);
private:
	//Window handles:
	GLFWwindow* m_Window ;
	WindowData m_WindowData;
	//Particle data:
	std::vector<Particle>* m_InstanceData;
	unsigned int m_InstanceCount;
	//GL handles:
	unsigned int m_VAO = 0, m_VBO = 0, m_InstanceVBO = 0;
	Shader* m_Shader;
	//Shader parameters:
	float m_pxSmoothness = 2.0f, m_quadSize = 10.0f, m_L = 1.0f;

	void Init();
};