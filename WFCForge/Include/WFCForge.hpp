#pragma once

#include "Core/Core.hpp"
#include "Core/Utils.hpp"
#include "Core/Window.hpp"
#include "Core/Application.hpp"

#include "UI/Style.hpp"
#include "UI/Dockspace.hpp"

#include "Graphics/Framebuffer.hpp"
#include "Graphics/Shader.hpp"
#include "Graphics/Camera.hpp"
#include "Graphics/Mesh.hpp"
#include "Graphics/MeshGen.hpp"
#include "Graphics/SSBO.hpp"
#include "Graphics/Texture2D.hpp"

#include "Editors/Viewport.hpp"

#include "Generators/TiledModel2D/TiledModel2DManager.hpp"
#include "Generators/OverlappedModel2D/OverlappedModel2DManager.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include <imgui_internal.h>
#include <imgui.h>