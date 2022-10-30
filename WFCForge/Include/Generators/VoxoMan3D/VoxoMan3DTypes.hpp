#pragma once

#include <glm/glm.hpp>

namespace WFCForge
{
	struct VoxoMan3DVoxel
	{
		glm::vec3 position;
		glm::vec3 size;
		glm::vec3 color;


		VoxoMan3DVoxel(glm::vec3 a, glm::vec3 b, glm::vec3 c)
			:position(a), size(b), color(c)
		{}
	};
}