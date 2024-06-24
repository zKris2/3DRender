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
	if (m_left_mouse_down)
	{
		float xoffset = xpos - m_current_x;
		float yoffset = m_current_y - ypos; // 注意这里要反转y坐标


		xoffset *= m_sensitivity;
		yoffset *= m_sensitivity;

		yaw += xoffset;
		pitch += yoffset;

		// 限制俯仰角，防止视角翻转
		if (pitch > 89.0f)
			pitch = 89.0f;
		if (pitch < -89.0f)
			pitch = -89.0f;
	}
}
void CameraControl::on_key(int key, int action, int mod)
{
	//std::cout << "on_key" << std::endl;
	if (action == GLFW_REPEAT)
	{
		return;
	}
	bool pressed = action == GLFW_PRESS ? true : false;

	m_keymap[key] = pressed;
}

void CameraControl::update()
{
	float radius = sqrt((0.0f - 0.0f) * (0.0f - 0.0f) + (5.0f - 0.0f) * (5.0f - 0.0f));
	float x = radius * sin(yaw);
	m_camera->m_position.x = glm::radians(yaw);
	m_camera->m_position.y = glm::radians(pitch);
	//std::cout << m_camera->m_position.x << std::endl;
}