#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include "application/imgui/imgui.h"
#include "application/imgui/imgui_impl_glfw.h"
#include "application/imgui/imgui_impl_opengl3.h"

#include <learnopengl/shader.h>
#include<learnopengl/animation.h>
#include<learnopengl/animator.h>

#include"Application/camera/camera.h"

#include<thread>

const unsigned int WINDOW_WIDTH = 1920;
const unsigned int WINDOW_HEIGHT = 1080;

void prepareTexture(const std::string& filename, unsigned int uint = 0);
void window_mouse(GLFWwindow* window, int button, int action, int mods);
void cursor_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);


Camera camera;
float deltaTime = 0.0f;
float lastFrame = 0.0f;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_SAMPLES, 4);//mutismaple anti-aliasing

	GLFWwindow*  window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "3DRender", nullptr, nullptr);
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
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_MULTISAMPLE);
	//glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetScrollCallback(window, scroll_callback);

	//CAMERA
	camera.set_init_position(glm::vec3(0.0f, 1.0f, 5.0f));

	//VAO VBO
	float cube[] = {
		// positions          // normals           // texture coords
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
	};
	glm::vec3 light_position[] = {
		glm::vec3(1.0f, 2.0f, -1.0f),
	};

	//nanosuit
	Shader model_shader("assets/shaders/model.vert", "assets/shaders/model.frag");
	Model assimp_model = Model("assets/model/nanosuit/nanosuit.obj");
	float model_angle=0.0f;

	//light
	Shader light_shader("assets/shaders/light.vert", "assets/shaders/light.frag");
	glm::vec3 light_color(1.0f);//attribute
	float light_angle = 0.0f;
	float light_radius = glm::distance(light_position[0], glm::vec3(0, light_position[0].y, 0));;
	float rotationSpeed = 1; // rotationSpeed 是旋转速度的系数
	

	GLuint light_vao, light_vbo;

	glGenVertexArrays(1, &light_vao);
	glGenBuffers(1, &light_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, light_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glBindVertexArray(light_vao);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);

	glBindVertexArray(0);

	// IMGUI
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	(void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 460");

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);
		
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		model_shader.begin();
		glm::mat4 model(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
		model = glm::scale(model, glm::vec3(0.2f));
		model = glm::rotate(model, model_angle, glm::vec3(0.0f, 1.0f, 0.0f));
		model_shader.setMatrix4("model", model);
		model_shader.setMatrix4("view", camera.get_camera_matrix());
		glm::mat4 projection(1.0f);
		projection = glm::perspective(glm::radians(camera.m_zoom),(float)WINDOW_WIDTH/(float)WINDOW_HEIGHT,0.1f,1000.0f);
		model_shader.setMatrix4("projection", projection);
		model_shader.setVec3("view_position", camera.m_position);
		model_shader.setVec3("point_light.color", light_color);
		model_shader.setVec3("point_light.position", light_position[0]);
		model_shader.setVec3("point_light.ambient", 0.4f, 0.4f, 0.4f);
		model_shader.setVec3("point_light.diffuse", 0.8f, 0.8f, 0.8f);
		model_shader.setVec3("point_light.specular", 1.0f, 1.0f, 1.0f);
		model_shader.setFloat("point_light.constant", 1.0f);
		model_shader.setFloat("point_light.linear", 0.09f);
		model_shader.setFloat("point_light.quadratic", 0.032f);
		assimp_model.Draw(model_shader);
		model_shader.end();

		light_shader.begin();
		glm::mat4 projection2(1.0f);
		projection2 = glm::perspective(glm::radians(camera.m_zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
		light_shader.setMatrix4("projection", projection2);
		light_shader.setMatrix4("view", camera.get_camera_matrix());
		light_shader.setVec3("light_color", light_color);
		glBindVertexArray(light_vao);
		for (int i = 0; i < sizeof(light_position)/sizeof(light_position[0]); i++)
		{
			light_position[i].x = light_radius * sin(light_angle * rotationSpeed);
			light_position[i].z = light_radius * cos(light_angle * rotationSpeed);
		
			glm::mat4 model2(1.0f);
			model2 = glm::translate(model2, light_position[i]);
			model2 = glm::scale(model2, glm::vec3(0.1f));
			model2 = glm::rotate(model2, 0.0f, glm::vec3(0.0f, 1.0f, 0.0f));
			light_shader.setMatrix4("model", model2);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		
		light_shader.end();

		// Start the Dear ImGui frame
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGui::Begin("IMGUI");
			ImGui::Text("Come on,Let's do it!");
			ImGui::SliderFloat("model-angle", &model_angle, -10*3.14f, 10*3.14f);
			ImGui::SliderFloat("light-angle", &light_angle, -6.28f, 6.28f);
			ImGui::SliderFloat("light-radius", &light_radius, 1, 5.0f);
			ImGui::ColorEdit3("light-color", (float*)&light_color);
		ImGui::End();
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	// Cleanup
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}


void prepareTexture(const std::string& filename, unsigned int uint) {
	GLuint texture;
	int img_width, img_height;
	int channels;
	stbi_set_flip_vertically_on_load(true);

	unsigned char* data = stbi_load(filename.c_str(), &img_width, &img_height, &channels, STBI_rgb_alpha);

	glGenTextures(1, &texture);
	glad_glActiveTexture(GL_TEXTURE0 + uint);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, img_width, img_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);

	stbi_image_free(data);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

void window_mouse(GLFWwindow* window, int button, int action, int mods) {
	double x;
	double y;
	glfwGetCursorPos(window, &x, &y);
	camera.on_mouse(button, action, x, y);
}
void cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera.on_cursor(xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.on_scroll(yoffset);
}
void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera.on_key(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera.on_key(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera.on_key(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera.on_key(RIGHT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
		camera.on_key(UP, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS)
		camera.on_key(DOWN, deltaTime);
}
