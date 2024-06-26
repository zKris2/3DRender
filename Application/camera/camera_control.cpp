#include"camera_control.h"

CameraControl::CameraControl()
{
}

CameraControl::~CameraControl()
{
}

void CameraControl::on_mouse(int button, int action, double xpos, double ypos)
{
	//std::cout << "on_mouse" << std::endl;

	bool pressed = action == GLFW_PRESS ? true : false;

	if (pressed)
	{
		m_current_x = xpos;
		m_current_y = ypos;
	}

	switch (button)
	{
	case GLFW_MOUSE_BUTTON_LEFT:
	{
		m_left_mouse_down = pressed;
		break;
	}
	case GLFW_MOUSE_BUTTON_RIGHT:
		m_right_mouse_down = pressed;
		break;
	case GLFW_MOUSE_BUTTON_MIDDLE:
		m_middle_mouse_down = pressed; 
		break;
	}

	
}
void CameraControl::on_cursor(double xpos, double ypos)
{
	//std::cout << "on_cursor" << std::endl;

	float xposOut = static_cast<float>(xpos);
	float yposOut = static_cast<float>(ypos);

	if (firstMouse)
	{
		lastX = xposOut;
		lastY = yposOut;
		firstMouse = false;
	}

	float xoffset = xposOut - lastX;
	float yoffset = lastY - yposOut; // reversed since y-coordinates go from bottom to top
	lastX = xposOut;
	lastY = yposOut;

	xoffset *= m_sensitivity;
	yoffset *= m_sensitivity;

	m_camera->m_yaw += xoffset;
	m_camera->m_pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (m_camera->m_pitch > 89.0f)
		m_camera->m_pitch = 89.0f;
	if (m_camera->m_pitch < -89.0f)
		m_camera->m_pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(m_camera->m_yaw)) * cos(glm::radians(m_camera->m_pitch));
	front.y = sin(glm::radians(m_camera->m_pitch));
	front.z = sin(glm::radians(m_camera->m_yaw)) * cos(glm::radians(m_camera->m_pitch));
	m_camera->m_front = glm::normalize(front);
}
void CameraControl::on_key(int key, int action, int mod)
{
	//std::cout << "on_key ----> " << key << std::endl;
	float cameraSpeed = m_sensitivity;
	if (key == GLFW_KEY_W)
		m_camera->m_position += cameraSpeed * m_camera->m_front;
	if (key == GLFW_KEY_S)
		m_camera->m_position -= cameraSpeed * m_camera->m_front;
	if (key == GLFW_KEY_A)
		m_camera->m_position -= glm::normalize(glm::cross(m_camera->m_front, m_camera->m_up)) * cameraSpeed;
	if (key == GLFW_KEY_D)
		m_camera->m_position += glm::normalize(glm::cross(m_camera->m_front, m_camera->m_up)) * cameraSpeed;
}

void CameraControl::on_scroll(double xoffset, double yoffset)
{
	auto pers_camera = dynamic_cast<PerspectiveCamera*>(m_camera);
	pers_camera->m_fovy -= (float)yoffset;
	if (pers_camera->m_fovy < 1.0f)
		pers_camera->m_fovy = 1.0f;
	if (pers_camera->m_fovy > 45.0f)
		pers_camera->m_fovy = 45.0f;
	std::cout << pers_camera->m_fovy << std::endl;
}
