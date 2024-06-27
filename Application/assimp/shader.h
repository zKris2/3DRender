#pragma once
#include<glad/glad.h>
#include <GLFW/glfw3.h>
#include<string>
#include<fstream>
#include<sstream>
#include<iostream>
#include<memory>

#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

class Shader
{
public:
	using ptr = std::unique_ptr<Shader>;
	Shader(const char* vertex_path, const char* fragment_path);
	~Shader();

	void begin();
	void end();

	void setInt(const std::string& name, int value);
	void setFloat(const std::string& name, float value);
	void setMatrix4(const std::string& name, glm::mat4 value);
	void setVec3(const std::string& name, float x, float y, float z) const;
	void setVec3(const std::string& name, glm::vec3& value) const;
private:
	void checkShaderError(GLuint target, std::string type);

public:
	GLuint m_shader_program;
};
