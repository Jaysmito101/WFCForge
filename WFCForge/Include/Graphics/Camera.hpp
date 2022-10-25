#pragma once
#include "Core/Core.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace WFCForge
{

	class Camera
	{
	public:
		Camera() = default;
		virtual ~Camera() = default;

		void SetPrespective(float aspect = 16.0f / 9.0f, float fov = 45.0f, float cNear = 0.001f, float cFar = 10000.0f);
		void SetOrthographic();

		void RecalculateMatrices();

		inline glm::mat4& GetProjection() { return this->projection; }
		inline glm::mat4& GetView() { return this->view; }
		inline glm::mat4& GetPV() { return this->pv; }

		inline float* GetProjectionPTR() { return glm::value_ptr(this->projection); }
		inline float* GetViewPTR() { return glm::value_ptr(this->view); }
		inline float* GetPVPTR() { return glm::value_ptr(this->pv); }

		inline glm::vec3& GetPosition() { return this->position; }
		inline glm::vec3& GetRotation() { return this->rotation; }
		inline glm::vec3& GetViewDir() { return this->viewDir; }
		inline glm::vec3& GetUpDir() { return this->upDir; }
		inline glm::vec3& GetRightDir() { return this->rightDir; }

		inline float* GetPositionPTR() { return glm::value_ptr(this->position); }
		inline float* GetRotationPTR() { return glm::value_ptr(this->rotation); }
		inline float* GetViewDirPTR() { return glm::value_ptr(this->viewDir); }
		inline float* GetUpDirPTR() { return glm::value_ptr(this->upDir); }
		inline float* GetRightDirPTR() { return glm::value_ptr(this->rightDir); }

		inline bool IsPerspective() { return this->isPerspective; }
		inline bool IsOrthographic() { return !this->isPerspective; }

	private:
		glm::mat4 projection = glm::mat4(1.0f);
		glm::mat4 view = glm::mat4(1.0f);
		glm::mat4 pv = glm::mat4(1.0f);
		bool isPerspective = true;

		glm::vec3 position = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 rotation = glm::vec3(0.0f);
		glm::vec3 viewDir = glm::vec3(0.0f);
		glm::vec3 upDir = glm::vec3(0.0f);
		glm::vec3 rightDir  = glm::vec3(0.0f);
	};

}