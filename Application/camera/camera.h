#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

class Camera
{
public:
	Camera();
	~Camera();

	glm::mat4 get_camera_matrix();
	virtual glm::mat4 get_project_matrix();
public:
	glm::vec3 m_position;
	glm::vec3 m_up;
	glm::vec3 m_right;
};
