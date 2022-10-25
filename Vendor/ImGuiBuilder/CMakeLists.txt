include_directories(
	../ImGui
	../ImGui/backends
	)


set(SOURCES
	# ImGui
    ../ImGui/imgui.cpp
    ../ImGui/imgui_demo.cpp
    ../ImGui/imgui_draw.cpp
    ../ImGui/imgui_tables.cpp
    ../ImGui/imgui_widgets.cpp
	)

add_library(ImGui ${SOURCES})