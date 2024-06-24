#include"camera.h"



Camera::Camera():m_position(0.0f,1.0f,2.0f),m_up(0.0f,1.0f,0.0f),m_right(1.0f,0.0f,0.0f)
	
{
}

Camera::~Camera()
{
}
glm::mat4 Camera::get_camera_matrix()
{
	//lookAt
	glm::vec3 front = glm::cross(m_up, m_right);
	//glm::vec3 center = m_position + front;
	glm::vec3 center = glm::vec3(0.0f,0.0f,0.0f);

	return glm::lookAt(m_position,center,m_up);
}

glm::mat4 Camera::get_project_matrix()
{
	return glm::identity<glm::mat4>();
}
