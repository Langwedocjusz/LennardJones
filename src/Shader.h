#include <iostream>
#include <string>

class Shader {
public:
	Shader(const std::string& vs_path, const std::string& fs_path);
	Shader(const std::string& compute_path);
	~Shader();

	void Bind();
private:
	unsigned int m_ID;
};