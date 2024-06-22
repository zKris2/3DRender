#include "shader.h"


Shader::Shader(const char* vertex_filename, const char* fragment_filename):m_shader_program(0)
{
	std::string vertex_code;
	std::string fragment_code;
	std::ifstream vertex_shader_file;
	std::ifstream fragment_shader_file;

	vertex_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fragment_shader_file.exceptions(std::ifstream::failbit | std::ifstream::badbit);

	try {
		vertex_shader_file.open(vertex_filename);
		fragment_shader_file.open(fragment_filename);
		std::stringstream ver_ss, frg_ss;
		ver_ss << vertex_shader_file.rdbuf();
		frg_ss << fragment_shader_file.rdbuf();

		vertex_shader_file.close();
		fragment_shader_file.close();

		vertex_code = ver_ss.str();
		fragment_code = frg_ss.str();
	}
	catch (std::ifstream::failure& e) {
		std::cout << "ERROR : " << e.what() << "[" << __FILE__ << ":" << __LINE__ << "]" << std::endl;
	}

	const char* vertexShaderSource = vertex_code.c_str();
	const char* fragmentShaderSource = fragment_code.c_str();

	GLuint vertex, fragment;
	vertex = glCreateShader(GL_VERTEX_SHADER);
	fragment = glCreateShader(GL_FRAGMENT_SHADER);

	glShaderSource(vertex, 1, &vertexShaderSource, nullptr);
	glShaderSource(fragment, 1, &fragmentShaderSource, nullptr);

	//compile
	glCompileShader(vertex);
	checkShaderError(vertex, "COMPILE");
	glCompileShader(fragment);
	checkShaderError(fragment, "COMPILE");

	// create program
	m_shader_program = glCreateProgram();

	//attach
	glAttachShader(m_shader_program, vertex);
	glAttachShader(m_shader_program, fragment);

	//link
	glLinkProgram(m_shader_program);
	checkShaderError(m_shader_program, "LINK");

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader()
{
}
void Shader::begin()
{
	glUseProgram(m_shader_program);
}
void Shader::end()
{
	glUseProgram(0);
}

void Shader::setFloat(const std::string& name, float value)
{
	GLuint location = glGetUniformLocation(m_shader_program, name.c_str());
	glUniform1f(location, value);
}



void Shader::checkShaderError(GLuint target, std::string type)
{
	int result = 0;
	char infoLog[1024];
	if (type == "COMPILE")
	{
		glGetShaderiv(target, GL_COMPILE_STATUS, &result);
		if (!result)
		{
			glGetShaderInfoLog(target, 1024, nullptr, infoLog);
			std::cout << "FRAGMENT : SHADER COMPILE ERROR \n" << infoLog << std::endl;
		}
	}
	else if (type == "LINK")
	{
		glGetProgramiv(target, GL_LINK_STATUS, &result);
		if (!result)
		{
			glGetProgramInfoLog(target, 1024, nullptr, infoLog);
			std::cout << "PROGRAM : SHADER LINK ERROR \n" << infoLog << std::endl;
		}

	}
	else
	{
		std::cout << "ERROR : Check Shader Error Type Is Wrong" << std::endl;
	}
}

