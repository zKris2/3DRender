#pragma once
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>

class Shader
{
public:
	Shader(const char* vertex_path, const char* fragment_path);
	~Shader();

	void begin();
	void end();

	void setFloat(const std::string& name, float value);
private:
	void checkShaderError(GLuint target, std::string type);

private:
	GLuint m_shader_program;
};
