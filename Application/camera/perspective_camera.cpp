#include "perspective_camera.h"


PerspectiveCamera::PerspectiveCamera(float fovy, float aspect, float near, float far)
	:m_fovy(fovy),m_aspect(aspect),m_near(near),m_far(far)
{
}

PerspectiveCamera::~PerspectiveCamera()
{
}

glm::mat4 PerspectiveCamera::get_project_matrix()
{
	return glm::perspective(glm::radians(m_fovy), m_aspect, m_near, m_far);
}
