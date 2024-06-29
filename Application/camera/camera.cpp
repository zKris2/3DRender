#include"camera.h"

Camera::Camera(glm::vec3 position)
	:m_position(position),
	m_up(0.0f,1.0f,0.0f), 
	m_front(0.0f, 0.0f, -1.0f),
	m_zoom(ZOOM),
	m_sensitivity(SENSITIVITY),
	m_movement_speed(SPEED),
	m_pitch(PITCH),
	m_yaw(YAW)
{
}

Camera::~Camera()
{
}
glm::mat4 Camera::get_camera_matrix()
{
	glm::vec3 center = m_position + m_front;
	return glm::lookAt(m_position,center,m_up);
}

glm::mat4 Camera::get_project_matrix()
{
	return glm::identity<glm::mat4>();
}

void Camera::on_mouse(int button, int action, double xpos, double ypos)
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
void Camera::on_cursor(double xpos, double ypos)
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

	m_yaw += xoffset;
	m_pitch += yoffset;

	// make sure that when pitch is out of bounds, screen doesn't get flipped
	if (m_pitch > 89.0f)
		m_pitch = 89.0f;
	if (m_pitch < -89.0f)
		m_pitch = -89.0f;

	glm::vec3 front;
	front.x = cos(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	front.y = sin(glm::radians(m_pitch));
	front.z = sin(glm::radians(m_yaw)) * cos(glm::radians(m_pitch));
	m_front = glm::normalize(front);
}
void Camera::on_key(Camera_Movement direction, float deltaTime)
{
	//std::cout << "on_key ----> " << key << std::endl;
	float velocity = m_movement_speed * deltaTime;
	if (direction == FORWARD)
		m_position += m_front * velocity;
	if (direction == BACKWARD)
		m_position -= m_front * velocity;
	if (direction == LEFT)
		m_position -= glm::cross(m_front,m_up) * velocity;
	if (direction == RIGHT)
		m_position += glm::cross(m_front, m_up) * velocity;
	if (direction == UP)
		m_position.y += velocity;
	if (direction == DOWN)
		m_position.y -= velocity;
}

void Camera::on_scroll(double yoffset)
{
	m_zoom -= ((float)yoffset * m_movement_speed);
	if (m_zoom < 1.0f)
		m_zoom = 1.0f;
	if (m_zoom > 30.0f)
		m_zoom = 30.0f;
}
