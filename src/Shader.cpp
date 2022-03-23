#include "Shader.h"

#include <fstream>
#include <sstream>

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

}

Shader::~Shader() {
	glDeleteProgram(m_ID);
}

void Shader::Bind() {
	glUseProgram(m_ID);
}