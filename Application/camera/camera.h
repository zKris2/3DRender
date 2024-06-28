#pragma once
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>
#include<glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <iostream>

enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP
};

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 2.5f;
const float SENSITIVITY = 0.1f;
const float ZOOM = 30.0f;

class Camera
{
public:
	Camera(glm::vec3 position = glm::vec3(0.0f,0.5f,3.0f));
	~Camera();

	glm::mat4 get_camera_matrix();
	virtual glm::mat4 get_project_matrix();

	virtual void on_mouse(int button, int action, double xpos, double ypos);
	virtual void on_cursor(double xpos, double ypos);
	virtual void on_key(Camera_Movement direction, float deltaTime);
	virtual void on_scroll(double yoffset);

	void set_movement_speed(float speed) { m_movement_speed = speed; }

public:
	glm::vec3 m_position;
	glm::vec3 m_up;
	glm::vec3 m_front;
	bool m_left_mouse_down = false;
	bool m_right_mouse_down = false;
	bool m_middle_mouse_down = false;

	float m_zoom;
	float m_sensitivity;
	float m_movement_speed;

	float m_yaw;
	float m_pitch;

	bool firstMouse = true;
	float m_current_x = 0.0f;
	float m_current_y = 0.0f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
};
