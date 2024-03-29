#pragma once

#include <iostream>
#include <string>
#include <vector>

class Shader {
public:
	Shader(const std::string& vs_path, const std::string& fs_path);
	Shader(const std::string& compute_path);
	~Shader();

	void Bind();
	void setUniform1f(const std::string& name, float x);
	void setUniform1ui(const std::string& name, unsigned int x);

private:
	unsigned int m_ID = 0;

	unsigned int getUniformLocation(const std::string& name);
	std::vector<std::pair<std::string, unsigned int>> m_UniformCache;
};