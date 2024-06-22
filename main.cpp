#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>

GLuint g_vao;

GLuint g_vertex_shader;
GLuint g_fragment_shader;
GLuint g_shader_porgram;

const char* vertexShaderSource =
	"#version 460 core\n"
	"layout (location = 0) in vec3 aPos;\n"
	"void main()\n"
	"{\n"
	"   gl_Position = vec4(aPos, 1.0);\n"
	"}\0";
const char* fragmentShaderSource =
	"#version 330 core\n"
	"out vec4 FragColor;\n"
	"void main()\n"
	"{\n"
	"   FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
	"}\n\0";

void prepareData()
{
	/*float position[] = {
		-1.0f, -1.0f,  0.0f,
		 1.0f ,-1.0f,  0.0f,
		 0.0f, 1.0f,  0.0f,
	};*/
	float position[] = {
		-1.0f, -1.0f,  0.0f,
		 1.0f ,-1.0f,  0.0f,
		 -1.0f, 1.0f,  0.0f,
		 1.0f, 1.0f,  0.0f
	};
	float index[] = {
		0,1,2,
		2,1,3
	};

	//pos
	GLuint pos_vbo = 0;
	glGenBuffers(1, &pos_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(position), position, GL_STATIC_DRAW);
	//index-ebo
	GLuint index_ebo = 0;
	glGenBuffers(1, &index_ebo);
	glBindBuffer(GL_ARRAY_BUFFER, index_ebo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);


	//vao
	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);

	//bind description
	//pos
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(float)*3, (void*)0);

	//add-ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_ebo);

	glBindVertexArray(0);
}

void prepareShader()
{
	// compile shader
	int result = 0;
	//vertex
	g_vertex_shader = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(g_vertex_shader, 1, &vertexShaderSource, NULL);
	glCompileShader(g_vertex_shader);
	glGetShaderiv(g_vertex_shader, GL_COMPILE_STATUS, &result);
	//fragment
	g_fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(g_fragment_shader, 1, &fragmentShaderSource, NULL);
	glCompileShader(g_fragment_shader);
	glGetShaderiv(g_fragment_shader, GL_COMPILE_STATUS, &result);

	// create program
	g_shader_porgram = glCreateProgram();
	//attach
	glAttachShader(g_shader_porgram, g_vertex_shader);
	glAttachShader(g_shader_porgram, g_fragment_shader);
	//link
	glLinkProgram(g_shader_porgram);

	glDeleteShader(g_vertex_shader);
	glDeleteShader(g_fragment_shader);
}

int main()
{
	GLFWwindow* window = nullptr;
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(1080, 920, "Rotating-Album", nullptr, nullptr);
	if (window == nullptr)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "INFO:[LoadGL failed]\tFILE:[" << __FILE__<<":" << __LINE__ <<"]" << std::endl;
		return false;
	}

	glViewport(0, 0, 1080, 920);

	prepareData();
	prepareShader();

	while (!glfwWindowShouldClose(window))
	{
		// render
		glClearColor(0.0, 0.0, 0.0, 1.0);
		glClear(GL_COLOR_BUFFER_BIT);

		// draw
		glUseProgram(g_shader_porgram);
		glBindVertexArray(g_vao);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);

		// exchange cache
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	glfwTerminate();
	return 0;
}