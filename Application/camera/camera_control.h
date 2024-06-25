#pragma once
#include<map>
#include<iostream>

#include"camera.h"

class CameraControl
{
public:
	CameraControl();
	~CameraControl();

	virtual void on_mouse(int button, int action, double xpos, double ypos);
	virtual void on_cursor(double xpos, double ypos);
	virtual void on_key(int key, int action, int mod);

	void update();

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

	float m_key_remove = 0.0f;
};

