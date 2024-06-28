#include <iostream>
#include<glad/glad.h>
#include<GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

#include"Application/camera/camera.h"
#include"Application/assimp/shader.h"
#include"Application/assimp/model.h"

const unsigned int WINDOW_WIDTH = 1920;
const unsigned int WINDOW_HEIGHT = 1080;

//camera
Camera::ptr camera;

//Light Color
glm::vec3 light_color(1.0f, 1.0f, 1.0f);
glm::vec3 light_pos(1.0f, 1.5f, -2.5f);
// timing
float deltaTime = 0.0f;	// time between current frame and last frame
float lastFrame = 0.0f;

void prepareTexture(const std::string& filename,unsigned int uint = 0) {
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
	camera->on_mouse(button, action, x, y);
}
void cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
	camera->on_cursor(xpos, ypos);
}
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera->on_scroll(yoffset);
}

void processInput(GLFWwindow* window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->on_key(FORWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->on_key(BACKWARD, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
		camera->on_key(LEFT, deltaTime);
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
		camera->on_key(RIGHT, deltaTime);
}

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

	glfwSetCursorPosCallback(window, cursor_callback);
	glfwSetScrollCallback(window, scroll_callback);

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
	
	glm::vec3 cubePositions[] = {
	   glm::vec3(0.0f,  0.0f,  0.0f),
	   glm::vec3(2.0f,  5.0f, -15.0f),
	   glm::vec3(-1.5f, -2.2f, -2.5f),
	   glm::vec3(-3.8f, -2.0f, -12.3f),
	   glm::vec3(2.4f, -0.4f, -3.5f),
	   glm::vec3(-1.7f,  3.0f, -7.5f),
	   glm::vec3(1.3f, -2.0f, -2.5f),
	   glm::vec3(1.5f,  2.0f, -2.5f),
	   glm::vec3(1.5f,  0.2f, -1.5f),
	   glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	glm::vec3 pointLightPositions[] = {
		glm::vec3(0.7f,  0.2f,  2.0f),
		glm::vec3(2.3f, -3.3f, -4.0f),
		glm::vec3(-4.0f,  2.0f, -12.0f),
		glm::vec3(0.0f,  0.0f, -3.0f)
	};

	GLuint model_vao,light_vao;
	//vao
	glGenVertexArrays(1, &model_vao);
	glGenVertexArrays(1, &light_vao);

	//cube
	glBindVertexArray(model_vao);

	GLuint model_vbo = 0;
	glGenBuffers(1, &model_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, model_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, model_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));

	// light
	glBindVertexArray(light_vao);

	GLuint light_vbo = 0;
	glGenBuffers(1, &light_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, light_vbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cube), cube, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, light_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);

	glBindVertexArray(0);

	Shader model_shader2("assets/shaders/model_vertex.vert", "assets/shaders/model_fragment.frag");
	auto model_shader = std::make_unique<Shader>("assets/shaders/vertex.glsl", "assets/shaders/fragment.glsl");
	auto light_shader =std::make_unique<Shader>("assets/shaders/light_vertex.vert", "assets/shaders/light_fragment.frag");
	//camera
	camera = std::make_unique<Camera>();
	auto light_pos = glm::vec3(3.0f, 2.5f, -2.5f);

	auto model = std::make_unique<Model>("assets/model/nanosuit/nanosuit.obj");
	int tex_num = model->textures_loaded.size();

	prepareTexture("assets/textures/container2.png", tex_num+1);
	prepareTexture("assets/textures/container2_specular.png", tex_num+2);

	while (!glfwWindowShouldClose(window))
	{
		float currentFrame = static_cast<float>(glfwGetTime());
		deltaTime = currentFrame - lastFrame;
		lastFrame = currentFrame;

		processInput(window);

		glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
		//glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		//model
		auto model_matrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model_matrix = glm::scale(model_matrix, glm::vec3(1.0f));
		model_matrix = glm::rotate(model_matrix, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
		auto perspective_matrix = glm::perspective(glm::radians(camera->m_zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);

		//light
		auto light_matrix = glm::translate(glm::mat4(1.0f), light_pos);
		light_matrix = glm::scale(light_matrix, glm::vec3(0.2f));

		//shader
		model_shader->begin();
		model_shader->setVec3("view_pos", camera->m_position);

		// directional light
		model_shader->setVec3("dirLight.direction", -0.2f, -1.0f, -0.3f);
		model_shader->setVec3("dirLight.ambient", 0.05f, 0.05f, 0.05f);
		model_shader->setVec3("dirLight.diffuse", 0.4f, 0.4f, 0.4f);
		model_shader->setVec3("dirLight.specular", 0.5f, 0.5f, 0.5f);
		// point light 1
		model_shader->setVec3("pointLights[0].position", pointLightPositions[0]);
		model_shader->setVec3("pointLights[0].ambient", 0.05f, 0.05f, 0.05f);
		model_shader->setVec3("pointLights[0].diffuse", 0.8f, 0.8f, 0.8f);
		model_shader->setVec3("pointLights[0].specular", 1.0f, 1.0f, 1.0f);
		model_shader->setFloat("pointLights[0].constant", 1.0f);
		model_shader->setFloat("pointLights[0].linear", 0.09f);
		model_shader->setFloat("pointLights[0].quadratic", 0.032f);
		// point light 2
		model_shader->setVec3("pointLights[1].position", pointLightPositions[1]);
		model_shader->setVec3("pointLights[1].ambient", 0.05f, 0.05f, 0.05f);
		model_shader->setVec3("pointLights[1].diffuse", 0.8f, 0.8f, 0.8f);
		model_shader->setVec3("pointLights[1].specular", 1.0f, 1.0f, 1.0f);
		model_shader->setFloat("pointLights[1].constant", 1.0f);
		model_shader->setFloat("pointLights[1].linear", 0.09f);
		model_shader->setFloat("pointLights[1].quadratic", 0.032f);
		// point light 3
		model_shader->setVec3("pointLights[2].position", pointLightPositions[2]);
		model_shader->setVec3("pointLights[2].ambient", 0.05f, 0.05f, 0.05f);
		model_shader->setVec3("pointLights[2].diffuse", 0.8f, 0.8f, 0.8f);
		model_shader->setVec3("pointLights[2].specular", 1.0f, 1.0f, 1.0f);
		model_shader->setFloat("pointLights[2].constant", 1.0f);
		model_shader->setFloat("pointLights[2].linear", 0.09f);
		model_shader->setFloat("pointLights[2].quadratic", 0.032f);
		// point light 4
		model_shader->setVec3("pointLights[3].position", pointLightPositions[3]);
		model_shader->setVec3("pointLights[3].ambient", 0.05f, 0.05f, 0.05f);
		model_shader->setVec3("pointLights[3].diffuse", 0.8f, 0.8f, 0.8f);
		model_shader->setVec3("pointLights[3].specular", 1.0f, 1.0f, 1.0f);
		model_shader->setFloat("pointLights[3].constant", 1.0f);
		model_shader->setFloat("pointLights[3].linear", 0.09f);
		model_shader->setFloat("pointLights[3].quadratic", 0.032f);
		// spotLight
		model_shader->setVec3("spotLight.position", camera->m_position);
		model_shader->setVec3("spotLight.direction", camera->m_front);
		model_shader->setVec3("spotLight.ambient", 0.0f, 0.0f, 0.0f);
		model_shader->setVec3("spotLight.diffuse", 1.0f, 1.0f, 1.0f);
		model_shader->setVec3("spotLight.specular", 1.0f, 1.0f, 1.0f);
		model_shader->setFloat("spotLight.constant", 1.0f);
		model_shader->setFloat("spotLight.linear", 0.09f);
		model_shader->setFloat("spotLight.quadratic", 0.032f);
		model_shader->setFloat("spotLight.cutOff", glm::cos(glm::radians(12.5f)));
		model_shader->setFloat("spotLight.outerCutOff", glm::cos(glm::radians(15.0f)));

		model_shader->setMatrix4("model_matrix", model_matrix);
		model_shader->setMatrix4("camera_matrix", camera->get_camera_matrix());
		model_shader->setMatrix4("perspective_matrix", perspective_matrix);

		model_shader->setInt("material.diffuse", tex_num + 1);
		model_shader->setInt("material.specular", tex_num + 2);
		model_shader->setFloat("material.shininess", 32.0f);

		//glm::vec3 diffuseColor = light_color * glm::vec3(0.5f);
		//glm::vec3 ambientColor = diffuseColor * glm::vec3(0.2f);
		model_shader->setVec3("light.ambient", glm::vec3(0.1f));
		model_shader->setVec3("light.diffuse", glm::vec3(0.8f)); 

		model_shader->setVec3("light.specular", 1.0f, 1.0f, 1.0f);
		model_shader->setFloat("light.constant", 1.0f);
		model_shader->setFloat("light.linear", 0.09f);
		model_shader->setFloat("light.quadratic", 0.032f);
		
		for (unsigned int i = 0; i < 10; i++)
		{
			glm::mat4 model(1.0f);
			model = glm::translate(model, cubePositions[i]);
			float angle = 20.0f * i;
			model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
			model_shader->setMatrix4("model_matrix", model);
			glBindVertexArray(model_vao);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		model_shader->end();

		auto model_matrix2 = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		model_matrix2 = glm::scale(model_matrix2, glm::vec3(0.5f));
		model_matrix2 = glm::rotate(model_matrix2, glm::radians(0.0f), glm::vec3(0.0, 1.0, 0.0));
		auto perspective_matrix2 = glm::perspective(glm::radians(camera->m_zoom), (float)WINDOW_WIDTH / (float)WINDOW_HEIGHT, 0.1f, 1000.0f);
		model_shader2.begin();
		model_shader2.setMatrix4("model", model_matrix2);
		model_shader2.setMatrix4("view", camera->get_camera_matrix());
		model_shader2.setMatrix4("projection", perspective_matrix2);
		model->Draw(model_shader2);
		model_shader2.end();
		

		// light shader
		/*light_shader->begin();
		light_shader->setMatrix4("model_matrix", light_matrix);
		light_shader->setMatrix4("camera_matrix", camera->get_camera_matrix());
		light_shader->setMatrix4("perspective_matrix", perspective_matrix);
		glBindVertexArray(light_vao);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		light_shader->end();*/
		
		light_shader->begin();
		light_shader->setMatrix4("camera_matrix", camera->get_camera_matrix());
		light_shader->setMatrix4("perspective_matrix", perspective_matrix);
		for (unsigned int i = 0; i < 4; i++)
		{
			auto model = glm::mat4(1.0f);
			model = glm::translate(model, pointLightPositions[i]);
			model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
			light_shader->setMatrix4("model_matrix", model);
			glBindVertexArray(light_vbo);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
		light_shader->end();

		// exchange cache
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}