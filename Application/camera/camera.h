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
	glm::vec3 m_front;

	float m_yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float m_pitch = 0.0f;
};
