#pragma once
#include"camera.h"


class PerspectiveCamera:public Camera
{
public:
	PerspectiveCamera(float fovy = 0.0f, float aspect=0.0f, float near=0.0f, float far=0.0f);
	~PerspectiveCamera();
	glm::mat4 get_project_matrix() override;
private:
	float m_fovy;
	float m_aspect;
	float m_near;
	float m_far;
};

