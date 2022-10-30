#pragma once
#include "Core/Core.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace WFCForge
{
	enum Uniform
	{
		Uniform_uTime = 0,
		Uniform_uPV,
		Uniform_uModel,
		Uniform_uFloorRes,
		Uniform_uMousePickID0,
		Uniform_uMousePickID1,
		Uniform_uMousePickID2,
		Uniform_uCameraPosition,
		Uniform_uTexture0,
		Uniform_uTexture1,
		Uniform_uTexture2,
		Uniform_uTexture3,
		Uniform_uTexture4,
		Uniform_uOffset0,
		Uniform_uLightPosition,
		Uniform_Count
	};

	static const char* UniformNames[] = 
	{
		"uTime",
		"uPV",
		"uModel",
		"uFloorRes",
		"uMousePickID0",
		"uMousePickID1",
		"uMousePickID2",
		"uCameraPosition",
		"uTexture0",
		"uTexture1",
		"uTexture2",
		"uTexture3",
		"uTexture4",
		"uOffset0",
		"uLightPosition"
	};

	class Shader
	{
	public:
		Shader() = default;
		virtual ~Shader() = default;


		bool Build(const std::string& vertexSource, const std::string& fragmentSource);
		void Destroy();
		void Bind();

		void SetFloat(Uniform name, const float data);
		void SetInteger(Uniform name, const int data);
		void SetIVector2(Uniform name, const int* data);
		void SetIVector3(Uniform name, const int* data);
		void SetIVector4(Uniform name, const int* data);
		void SetVector2(Uniform name, const glm::vec2& data);
		void SetVector3(Uniform name, const glm::vec3& data);
		void SetVector4(Uniform name, const glm::vec4& data);
		void SetMatrix3(Uniform name, const glm::mat3& data);
		void SetMatrix4(Uniform name, const glm::mat4& data);

		inline uint32_t GetHandle() { return this->handle; }
		inline bool IsBuild() { return this->isBuild; }

	private:
		void LoadUniforms();

	private:
		bool isBuild = false;
		uint32_t handle = 0;
		int32_t uniformLocations[Uniform_Count];
	};

}
