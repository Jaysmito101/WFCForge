#include "Core/Core.hpp"
#include "Graphics/Camera.hpp"

namespace WFCForge
{
	static const glm::vec3 FRONT = glm::vec3(0.0f, 0.0f, 1.0f);
	static const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);

	void Camera::SetPrespective(float aspect, float fov, float cNear, float cFar)
	{
		this->isPerspective = true;
		this->projection = glm::perspective(fov, aspect, cNear, cFar);
		this->RecalculateMatrices();
	}

	void Camera::SetOrthographic()
	{
		WFC_ASSERT(false, "Not yet implemented");
	}

	void Camera::RecalculateMatrices()
	{
		this->view = glm::lookAt(this->position, this->position + FRONT, UP);
		this->view = glm::rotate(this->view, rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
		this->view = glm::rotate(this->view, rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
		this->view = glm::rotate(this->view, rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));

		this->viewDir = glm::vec3(this->view[0].z, this->view[1].z, this->view[2].z);
		//this->upDir = glm::vec3(this->view[0].y, this->view[1].y, this->view[2].y);
		//this->view = glm::lookAt(this->position, this->position + this->viewDir, UP);
		this->upDir = glm::vec3(this->view[0].y, this->view[1].y, this->view[2].y);
		this->rightDir = glm::vec3(this->view[0].x, this->view[1].x, this->view[2].x);

		this->pv = this->projection * this->view;
	}

}