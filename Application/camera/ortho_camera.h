#pragma once
#include"camera.h"

class OrthoCamera :public Camera
{
public:
	OrthoCamera(float l = 0.0f, float r = 0.0f, float t = 0.0f, float b = 0.0f, float n = 0.0f, float f = 0.0f);
	~OrthoCamera();
	glm::mat4 get_project_matrix() override;
private:
	float m_left;
	float m_right;
	float m_bottom;
	float m_top;
	float m_near;
	float m_far;
};
