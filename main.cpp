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
#include"Application/camera/perspective_camera.h"
#include"Application/camera/camera_control.h"

GLFWwindow* window = nullptr;

#define WINDOW_WIDTH 1920.0
#define WINDOW_HEIGHT 1080.0


//VAO
GLuint g_vao;
//Shader
Shader* shader;
// Texture
GLuint g_texture;
int img_width, img_height;
//camera
PerspectiveCamera* camera = nullptr;
CameraControl* camera_control = nullptr;
//Matrix
glm::mat4 model_matrix(1.0f);
glm::mat4 perspective_matrix(1.0f);

void prepareData()
{
	float w_ratio = (float)((float)img_width / (float)WINDOW_WIDTH);
	float h_ratio = (float)((float)img_height / (float)WINDOW_HEIGHT);
	std::cout << "img_width:" << img_width << std::endl;
	std::cout << "img_height:" << img_height << std::endl;
	std::cout << "w_ratio:" << w_ratio << std::endl;
	std::cout << "h_ratio:" << h_ratio << std::endl;
	float position[] = {
		-w_ratio , -h_ratio,  0.0f,
		 w_ratio , -h_ratio,  0.0f,
		-w_ratio ,  h_ratio,  0.0f,
		 w_ratio ,  h_ratio,  0.0f
	};
	/*float position[] = {
		- w_ratio * 0.5f, - h_ratio * 0.5f,  0.0f,
		  w_ratio * 0.5f, - h_ratio * 0.5f,  0.0f,
		 - w_ratio * 0.5f,  h_ratio * 0.5f,  0.0f,
		  w_ratio * 0.5f,  h_ratio * 0.5f,  0.0f
	};*/
	float color[] = {
		-w_ratio * 0.5, -0,  0.0f,
		 w_ratio * 0.5, -0,  0.0f,
		-w_ratio * 0.5,  h_ratio,  0.0f,
		 w_ratio * 0.5,  h_ratio,  0.0f
	};

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


void prepareCamera()
{
	camera = new PerspectiveCamera(60.f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
	camera_control = new CameraControl();
	camera_control->set_camera(camera);
}

float angles = 1.0f;
void perpareMatrix()
{
	glm::mat4 scale_matrix(1.0f);
	glm::mat4 transform_matrix(1.0f);
	glm::mat4 rotate_matrix(1.0f);
	//fovy:张角
	//aspect:横纵比
	//zNear:近平面距离（与相机的距离，所以是正数）
	//zFar:远平面距离
	perspective_matrix = glm::perspective(glm::radians(60.0f), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.01f, 1000.0f);
	
	transform_matrix = glm::translate(scale_matrix, glm::vec3(0.0f, 0.0f, 0.5f));
	scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(0.5, 0.5, 1.0f));
	rotate_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(angles), glm::vec3(0.0, 1.0, 0.0));
	model_matrix = rotate_matrix * transform_matrix ;
}


void window_key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	camera_control->on_key(key, action, mods);
}
void window_mouse(GLFWwindow* window, int button, int action, int mods) {
	double x;
	double y;
	glfwGetCursorPos(window, &x, &y);
	camera_control->on_mouse(button, action, x, y);
}
void window_cursor(GLFWwindow* window, double xpos, double ypos)
{
	camera_control->on_cursor(xpos, ypos);
}

void bind_events()
{
	glfwSetKeyCallback(window, window_key);
	glfwSetMouseButtonCallback(window, window_mouse);
	glfwSetCursorPosCallback(window, window_cursor);
}


int main()
{
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
	
	prepareCamera();
	

	bind_events();

	glClearColor(0.0, 0.0, 0.0, 1.0);

	while (!glfwWindowShouldClose(window))
	{
		// exchange cache
		glfwSwapBuffers(window);
		glfwPollEvents();

		// clear buffer
		glClear(GL_COLOR_BUFFER_BIT);

		camera_control->update();
		perpareMatrix();

		//shader
		shader->begin();
		shader->setFloat("smapler", 0);
		shader->setMatrix4("model_matrix", model_matrix);
		shader->setMatrix4("camera_matrix", camera->get_camera_matrix());
		shader->setMatrix4("perspective_matrix", perspective_matrix);

		//bind vao
		glBindVertexArray(g_vao);

		//draw
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

		shader->end();
	}
	glfwTerminate();
	return 0;
}