#pragma once
#include<map>
#include<iostream>

#include"camera.h"
#include"perspective_camera.h"

class CameraControl
{
public:
	CameraControl();
	~CameraControl();

	virtual void on_mouse(int button, int action, double xpos, double ypos);
	virtual void on_cursor(double xpos, double ypos);
	virtual void on_key(int key, int action, int mod);
	virtual void on_scroll(double xoffset, double yoffset);

	void set_camera(Camera* camera) { m_camera = camera; }
	void set_sensitivity(float sentitivity) { m_sensitivity = sentitivity; }
private:
	bool m_left_mouse_down = false;
	bool m_right_mouse_down = false;
	bool m_middle_mouse_down = false;
	
	float m_current_x = 0.0f;
	float m_current_y = 0.0f;

	std::map<int, bool> m_keymap;

	float m_sensitivity = 0.2f;
	Camera* m_camera = nullptr;

	bool firstMouse = true;
	float yaw = -90.0f;	// yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right so we initially rotate a bit to the left.
	float pitch = 0.0f;
	float lastX = 800.0f / 2.0;
	float lastY = 600.0 / 2.0;
};

