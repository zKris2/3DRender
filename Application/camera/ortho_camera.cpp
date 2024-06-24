#include "ortho_camera.h"

OrthoCamera::OrthoCamera(float l, float r, float t, float b, float n, float f):m_left(l), m_right(r), m_top(t), m_bottom(b), m_near(n), m_far(f)
{
}

OrthoCamera::~OrthoCamera()
{
}

glm::mat4 OrthoCamera::get_project_matrix()
{
	return glm::ortho(m_left, m_right, m_bottom, m_top, m_near, m_far);
}
