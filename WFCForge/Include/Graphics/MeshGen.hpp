#pragma once

#include "Core/Core.hpp"
#include "Graphics/Mesh.hpp"

namespace WFCForge
{

	class MeshGen
	{
	private:
		MeshGen() = default;
		virtual ~MeshGen() = default;

	public:
		
		static void FloorTile(Mesh* mesh);
		static void Cube(Mesh* mesh);

	private:

	};

}
