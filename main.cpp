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

const unsigned int WINDOW_WIDTH = 1920.0;
const unsigned int WINDOW_HEIGHT = 1080.0;

//VAO
GLuint g_vao;
GLuint g_light_vao;
//Shader
Shader* shader;
Shader* light_shader;
// Texture
GLuint g_texture;
int img_width, img_height;
//camera
PerspectiveCamera* camera = nullptr;
float lastX = WINDOW_WIDTH / 2.0f;
float lastY = WINDOW_HEIGHT / 2.0f;
bool firstMouse = true;
CameraControl* camera_control = nullptr;
//Matrix
glm::mat4 model_matrix(1.0f);
glm::mat4 model_light_matrix(1.0f);
glm::mat4 perspective_matrix(1.0f);

//Light Color
glm::vec3 light_color(1.0f, 1.0f, 1.0f);
glm::vec3 light_pos(1.0f, 1.5f, -2.5f);

void prepareData()
{
	std::cout << "img_width:" << img_width << std::endl;
	std::cout << "img_height:" << img_height << std::endl;

	//cube
	/*float cube[] = {
		 -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};*/
	float cube[] = {
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,

	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f
	};

	//vao
	glGenVertexArrays(1, &g_vao);
	glBindVertexArray(g_vao);

	//cube
	GLuint cube_vbo = 0;
	glGenBuffers(1, &cube_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);
	
	//bind description
	glBindBuffer(GL_ARRAY_BUFFER, cube_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (void*)(3*sizeof(float)));
	
	// light-vao
	glGenVertexArrays(1, &g_light_vao);
	glBindVertexArray(g_light_vao);

	//light-vbo
	GLuint light_vbo = 0;
	glGenBuffers(1, &light_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, light_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	//bind description
	glBindBuffer(GL_ARRAY_BUFFER, light_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);

	glBindVertexArray(0);
}

void prepareTexture(const std::string& filename,unsigned int uint = 0) {
	int channels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(filename.c_str(), &img_width, &img_height, &channels, STBI_rgb_alpha);
	
	glGenTextures(1, &g_texture);
	glad_glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, g_texture);
	//send data to GPU
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
	light_shader = new Shader("assets/shaders/light_vertex.vert", "assets/shaders/light_fragment.frag");
}

void prepareCamera()
{
	camera = new PerspectiveCamera(45.0f, (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
	camera_control = new CameraControl();
	camera_control->set_camera(camera);
	camera_control->set_camera_pos(glm::vec3(0.0f,1.5f,4.0f));
	camera_control->set_sensitivity(0.1f);

}

float angles = 1.0f;
void perpareMatrix()
{
	glm::mat4 scale_matrix(1.0f);
	glm::mat4 transform_matrix(1.0f);
	glm::mat4 transform_light_matrix(1.0f);
	glm::mat4 rotate_matrix(1.0f);
	//fovy:张角
	//aspect:横纵比
	//zNear:近平面距离（与相机的距离，所以是正数）
	//zFar:远平面距离
	perspective_matrix = glm::perspective(glm::radians(dynamic_cast<PerspectiveCamera*>(camera)->m_fovy), dynamic_cast<PerspectiveCamera*>(camera)->m_aspect, dynamic_cast<PerspectiveCamera*>(camera)->m_near, dynamic_cast<PerspectiveCamera*>(camera)->m_far);
	
	transform_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
	scale_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(1.0f));
	rotate_matrix = glm::rotate(glm::mat4(1.0f), glm::radians(angles), glm::vec3(0.0, 1.0, 0.0));
	model_matrix = rotate_matrix * transform_matrix * scale_matrix;

	model_light_matrix = glm::translate(glm::mat4(1.0f), light_pos);
	model_light_matrix = glm::scale(model_light_matrix, glm::vec3(0.2f));
}


void window_key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);
	camera_control->on_key(key, action, mods);
}
void window_mouse(GLFWwindow* window, int button, int action, int mods) {
	double x;
	double y;
	glfwGetCursorPos(window, &x, &y);
	/*if (firstMouse)
	{
		x = lastX;
		y = lastY;
		firstMouse = false;
	}*/
	camera_control->on_mouse(button, action, x, y);
}
void window_cursor(GLFWwindow* window, double xpos, double ypos)
{
	camera_control->on_cursor(xpos, ypos);
}
void window_scroll(GLFWwindow* window, double xoffset, double yoffset)
{
	camera_control->on_scroll(xoffset, yoffset);
}

void bind_events()
{
	glfwSetKeyCallback(window, window_key);
	glfwSetMouseButtonCallback(window, window_mouse);
	glfwSetCursorPosCallback(window, window_cursor);
	glfwSetScrollCallback(window, window_scroll);
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
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	prepareTexture("assets/textures/container2.png", 0);
	prepareData();
	prepareShader();
	prepareCamera();

	//bind events
	bind_events();

	perpareMatrix();

	glEnable(GL_DEPTH_TEST);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		// clear buffer
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*light_color.x = sin(glfwGetTime() * 2.0f);
		light_color.y = sin(glfwGetTime() * 0.7f);
		light_color.z = sin(glfwGetTime() * 1.3f);*/
		glm::vec3 diffuseColor = light_color * glm::vec3(0.5f); // 降低影响
		glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f); // 很低的影响

		//shader
		shader->begin();
		shader->setInt("smapler", 0);
		shader->setMatrix4("model_matrix", model_matrix);
		shader->setMatrix4("camera_matrix", camera->get_camera_matrix());
		shader->setMatrix4("perspective_matrix", perspective_matrix);
	
		shader->setVec3("light_pos", light_pos);
		shader->setVec3("model_color", glm::vec3(0.5f, 0.2f, 0.1f));
		shader->setVec3("view_pos", camera_control->get_camer_pos());

		shader->setVec3("material.ambient", 1.0f, 0.5f, 0.31f);
		shader->setVec3("material.diffuse", 1.0f, 0.5f, 0.31f);
		shader->setVec3("material.specular", 0.5f, 0.5f, 0.5f);
		shader->setFloat("material.shininess", 32.0f);

		shader->setVec3("light.ambient", ambientColor);
		shader->setVec3("light.diffuse", diffuseColor); // 将光照调暗了一些以搭配场景
		shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);

		//bind vao
		glBindVertexArray(g_vao);
		//draw
		glDrawArrays(GL_TRIANGLES, 0, 36);
		shader->end();

		// light shader
		light_shader->begin();
		light_shader->setMatrix4("model_matrix", model_light_matrix);
		light_shader->setMatrix4("camera_matrix", camera->get_camera_matrix());
		light_shader->setMatrix4("perspective_matrix", perspective_matrix);
		glBindVertexArray(g_light_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		light_shader->end();

		// exchange cache
		glfwSwapBuffers(window);
		glfwPollEvents();
	}
	if (shader != nullptr)
		delete shader;
	if (camera != nullptr)
		delete camera;
	if (camera_control != nullptr)
		delete camera_control;

	glfwTerminate();
	return 0;
}