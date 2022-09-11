#include "Shader.h"

#include <fstream>
#include <sstream>
#include <algorithm>

#include "glad/glad.h"

Shader::Shader(const std::string& vs_path, const std::string& fs_path) {
	std::string vertexCode, fragmentCode;

	std::ifstream vertexFile(vs_path);
	std::ifstream fragmentFile(fs_path);

	if (!vertexFile)
		std::cout << "ERROR: VERTEX SHADER FILE NOT READ SUCCESFULLY: " << vs_path << std::endl;

	if (!fragmentFile)
		std::cout << "ERROR: FRAGMENT SHADER FILE NOT READ SUCCESFULLY: " << fs_path << std::endl;

	std::stringstream vertexStream, fragmentStream;
	vertexStream << vertexFile.rdbuf();
	fragmentStream << fragmentFile.rdbuf();

	vertexCode = vertexStream.str();
	fragmentCode = fragmentStream.str();

	const char* vCode = vertexCode.c_str();
	const char* fCode = fragmentCode.c_str();

	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vCode, NULL);
	glCompileShader(vertex);

	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR: VERTEX SHADER COMPILATION FAILED\n" << infoLog << std::endl;
	};

	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fCode, NULL);
	glCompileShader(fragment);

	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR: FRAGMENT SHADER COMPILATION FAILED\n" << infoLog << std::endl;
	};

	m_ID = glCreateProgram();
	glAttachShader(m_ID, vertex);
	glAttachShader(m_ID, fragment);
	glLinkProgram(m_ID);

	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
		std::cout << "ERROR: SHADER PROGRAM LINKING FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::Shader(const std::string& compute_path) {
	std::string computeCode;
	std::ifstream computeFile(compute_path);

	if (!computeFile)
		std::cout << "ERROR: COMPUTE SHADER FILE NOT READ SUCCESFULLY: " << compute_path << std::endl;

	std::stringstream computeStream;
	computeStream << computeFile.rdbuf();
	computeCode = computeStream.str();
	const char* cCode = computeCode.c_str();

	unsigned int compute;
	int success;
	char infoLog[512];
	compute = glCreateShader(GL_COMPUTE_SHADER);
	glShaderSource(compute, 1, &cCode, NULL);
	glCompileShader(compute);

	glGetShaderiv(compute, GL_COMPILE_STATUS, &success);
	if (!success) {
		glGetShaderInfoLog(compute, 512, NULL, infoLog);
		std::cout << "ERROR: COMPUTE SHADER COMPILATION FAILED\n" << infoLog << std::endl;
	};

	m_ID = glCreateProgram();
	glAttachShader(m_ID, compute);
	glLinkProgram(m_ID);

	glGetProgramiv(m_ID, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(m_ID, 512, NULL, infoLog);
		std::cout << "ERROR: SHADER PROGRAM LINKING FAILED\n" << infoLog << std::endl;
	}

	glDeleteShader(compute);
}

Shader::~Shader() {
	glDeleteProgram(m_ID);
}

void Shader::Bind() {
	glUseProgram(m_ID);
}

unsigned int Shader::getUniformLocation(const std::string& name) {
	auto search_result = std::find_if(m_UniformCache.begin(), m_UniformCache.end(),
		[&name](const std::pair<std::string, unsigned int>& element) { 
			return element.first == name; 
		});

	if (search_result != m_UniformCache.end())
		return search_result->second;

	const unsigned int location = glGetUniformLocation(m_ID, name.c_str());
	m_UniformCache.push_back(std::make_pair(name, location));
	return location;
}

void Shader::setUniform1f(const std::string& name, float x) {
	const unsigned int location = getUniformLocation(name.c_str());
	glUniform1f(location, x);
}

void Shader::setUniform1ui(const std::string& name, unsigned int x) {
	const unsigned int location = getUniformLocation(name.c_str());
	glUniform1ui(location, x);
}
