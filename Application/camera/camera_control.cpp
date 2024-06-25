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
}
void CameraControl::on_key(int key, int action, int mod)
{
	std::cout << "on_key ----> " << key << std::endl;
	float cameraSpeed = m_sensitivity;
	auto cameraFront = glm::cross(m_camera->m_up, m_camera->m_right);
	if (key == GLFW_KEY_W)
		m_camera->m_position += cameraSpeed * cameraFront;
	if (key == GLFW_KEY_S)
		m_camera->m_position -= cameraSpeed * cameraFront;
	if (key == GLFW_KEY_A)
		m_camera->m_position -= m_camera->m_right * cameraSpeed;
	if (key == GLFW_KEY_D)
		m_camera->m_position += m_camera->m_right * cameraSpeed;
}

void CameraControl::update()
{
}