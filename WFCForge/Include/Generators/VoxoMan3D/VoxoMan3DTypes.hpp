#pragma once

#include <glm/glm.hpp>

#define WFC_RGB(x, y, z) {x/255.0f, y/255.0f, z/255.0f}

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

	struct VoxoMan3DContext
	{
		std::vector<glm::vec3> skinColorPalatte = {
			WFC_RGB(255, 219, 172),
			WFC_RGB(241,194,125),
			WFC_RGB(224,172,105),
			WFC_RGB(232, 190, 172),
			WFC_RGB(224, 172, 105),
			WFC_RGB(217, 167, 133),
			WFC_RGB(207, 152, 116),
			WFC_RGB(236, 198, 169)
		};

		glm::vec3 skinColor;
		bool isMale = true;
		int seed = 42;

		struct
		{
			glm::vec3 start;
			glm::vec3 size;
			glm::vec3 end;
			glm::vec3 prefferedSize = glm::vec3(0.17, 0.17, 0.17);
			glm::vec3 prefferedSizeLimits = glm::vec3(0.04, 0.02, 0.04);
			int seed = 42;
		} head;

		struct
		{
			glm::vec3 start;
			glm::vec3 end;
			glm::vec3 size;
			float height;
			float prefferredHeight = 0.07f;
			float heightLimits = 0.005f;
			float xySizeFactor;
			float prefferedXYSizeFactor = 0.6f;
			float xySizeFactorLimits = 0.1f;
			int seed = 42;
		} neck;

		struct
		{
			glm::vec3 start;
			glm::vec3 size;
			glm::vec3 end;
			glm::vec3 prefferredSize = glm::vec3(0.3, 0.5, 0.15);
			glm::vec3 prefferredSizeLimits = glm::vec3(0.05, 0.15, 0.04);
			glm::vec3 breastLeftStart;
			glm::vec3 breastRightStart;
			glm::vec3 breastSize;
			glm::vec3 breastSizePreffered = glm::vec3(0.1, 0.15, 0.1);
			glm::vec3 breastSizeLimits = glm::vec3(0.05, 0.05, 0.05);
			int seed = 42;
		} torso;

		struct
		{
			glm::vec3 startL;
			glm::vec3 endL;
			glm::vec3 startR;
			glm::vec3 endR;
			glm::vec3 size;
			glm::vec3 prefferredSize = glm::vec3(0.08, 0.5, 0.08);
			glm::vec3 prefferredSizeLimits = glm::vec3(0.01, 0.05, 0.01);
			int seed = 42;
		} arms;

		struct
		{
			glm::vec3 startL;
			glm::vec3 endL;
			glm::vec3 startR;
			glm::vec3 endR;
			glm::vec3 size;
			glm::vec3 prefferredSize = glm::vec3(0.1, 0.5, 0.1);
			glm::vec3 prefferredSizeLimits = glm::vec3(0.05, 0.1, 0.05);
			int seed = 42;
		} legs;

		struct
		{
			glm::vec3 startL;
			glm::vec3 endL;
			glm::vec3 startR;
			glm::vec3 endR;
			glm::vec3 size;
			glm::vec3 prefferredSize = glm::vec3(0.1, 0.1, 0.3);
			glm::vec3 prefferredSizeLimits = glm::vec3(0.001, 0.05, 0.05);
			int seed = 42;
		} feet;
	};
}