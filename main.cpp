#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include"stb_image.h"
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include"glframework/shader.h"

#define WINDOW_WIDTH 1920
#define WINDOW_HEIGHT 1080

//VAO
GLuint g_vao;
//Shader
Shader* shader;
// Texture
GLuint g_texture;
int img_width, img_height;
//Metrix
glm::mat4 camera_metrix(1.0f);
glm::mat4 prespective_metrix(1.0f);

void prepareData()
{
	/*float position[] = {
		-1.0f, -1.0f,  0.0f,
		 1.0f ,-1.0f,  0.0f,
		 0.0f, 1.0f,  0.0f,
	};*/
	/*float position[] = {
		-1.0f, -1.0f,  0.0f,
		 1.0f ,-1.0f,  0.0f,
		 -1.0f, 1.0f,  0.0f,
		 1.0f, 1.0f,  0.0f
	};*/
	/*float position[] = {
		-0.5f, -0.5f,  0.0f,
		 0.5f ,-0.5f,  0.0f,
		 -0.5f, 0.5f,  0.0f,
		 0.5f,  0.5f,  0.0f
	};*/
	float w_ratio = (float)((float)img_width / (float)WINDOW_WIDTH);
	float h_ratio = (float)((float)img_height / (float)WINDOW_HEIGHT);
	std::cout << "img_width:" << img_width << std::endl;
	std::cout << "img_height:" << img_height << std::endl;
	std::cout << "w_ratio:" << w_ratio << std::endl;
	std::cout << "h_ratio:" << h_ratio << std::endl;
	float position[] = {
		-w_ratio, -0,  0.0f,
		 w_ratio, -0,  0.0f,
		-w_ratio,  2*h_ratio,  0.0f,
		 w_ratio,  2*h_ratio,  0.0f
	};
	/*float position[] = {
		- w_ratio * 0.5f, - h_ratio * 0.5f,  0.0f,
		  w_ratio * 0.5f, - h_ratio * 0.5f,  0.0f,
		 - w_ratio * 0.5f,  h_ratio * 0.5f,  0.0f,
		  w_ratio * 0.5f,  h_ratio * 0.5f,  0.0f
	};*/

	float index[] = {
		0,1,2,
		2,1,3
	};
	float uvs[] = {
		0.0f,0.0f,
		1.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f
	};

	//pos
	GLuint pos_vbo = 0;
	glGenBuffers(1, &pos_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(position), position, GL_STATIC_DRAW);
	//uv
	GLuint uv_vbo = 0;
	glGenBuffers(1, &uv_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(uvs), uvs, GL_STATIC_DRAW);

	//index-ebo
	GLuint index_ebo = 0;
	glGenBuffers(1, &index_ebo);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(index), index, GL_STATIC_DRAW);
	
	//vao
	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);

	//bind description
	//pos-vbo
	glBindBuffer(GL_ARRAY_BUFFER, pos_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(float)*3, (void*)0);
	//index-ebo
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, index_ebo);
	//uv-vbo
	glBindBuffer(GL_ARRAY_BUFFER, uv_vbo);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 2, (void*)0);

	glBindVertexArray(0);
}

void prepareTexture(const std::string& filename,unsigned int uint = 0) {
	int channels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(filename.c_str(), &img_width, &img_height, &channels, STBI_rgb_alpha);
	
	glGenTextures(1, &g_texture);
	glad_glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_texture);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}
void prepareShader()
{
	shader = new Shader("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
}
void MetrixTransform() {
	camera_metrix = glm::lookAt(glm::vec3(0.0f, 0.5f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	prespective_metrix = glm::perspective(glm::radians(60.0f), (float)1080.f / (float)1920.f , 0.1f, 1000.0f);
}

int main()
{
	GLFWwindow* window = nullptr;
	
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Rotating-Album", nullptr, nullptr);
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

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

	prepareTexture("assets/images/2.png", 0);
	prepareData();
	prepareShader();
	MetrixTransform();

	glClearColor(0.0, 0.0, 0.0, 1.0);

	while (!glfwWindowShouldClose(window))
	{
		// exchange cache
		glfwSwapBuffers(window);
		glfwPollEvents();

		// clear buffer
		glClear(GL_COLOR_BUFFER_BIT);

		//shader
		shader->begin();
		shader->setFloat("smapler", 0);
		shader->setMatrix4("camera_matrix", camera_metrix);
		shader->setMatrix4("prespective_metrix", prespective_metrix);

		//bind vao
		glBindVertexArray(g_vao);

		//draw
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		shader->end();
	}
	glfwTerminate();
	return 0;
}