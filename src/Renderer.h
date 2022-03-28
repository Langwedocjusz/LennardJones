#include "Shader.h"

#include "GLFW/glfw3.h"

#include <string>

struct WindowData {
	std::string Title = "Lennard Jones";
	unsigned int Width = 800, Height = 600;
};

class Renderer {
public:
	Renderer(std::vector<float>* data);
	~Renderer();

	void OnRender();
	bool ShouldClose();

	void setSmoothness(float s) { m_pxSmoothness = s; }
	void setQuadSize(float s) { m_quadSize = s; }
private:
	GLFWwindow* m_Window ;
	WindowData m_WindowData;
	unsigned int m_VAO = 0, m_VBO = 0, m_InstanceVBO = 0;
	Shader* m_Shader;
	float m_pxSmoothness = 2.0f, m_quadSize = 10.0f;
	std::vector<float>* m_InstanceData;
	unsigned int m_InstanceCount;
};