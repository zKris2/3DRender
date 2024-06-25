#include"camera.h"



Camera::Camera():m_position(0.0f,0.0f,3.0f),m_up(0.0f,1.0f,0.0f), m_front(0.0f, 0.0f, -1.0f), m_fov(45.0f)
{
}

Camera::~Camera()
{
}
glm::mat4 Camera::get_camera_matrix()
{
	//lookAt
	glm::vec3 center = m_position + m_front;
	return glm::lookAt(m_position,center,m_up);
}

glm::mat4 Camera::get_project_matrix()
{
	return glm::identity<glm::mat4>();
}
