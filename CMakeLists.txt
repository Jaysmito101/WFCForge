cmake_minimum_required(VERSION 3.12)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

project(WFCForge C CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REUIRED True)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REUIRED True)

find_package( OpenGL REQUIRED )

include_directories( 
    ./WFCForge/Include
    ./Vendor/Glad/Include
    ./Vendor/GLFW/Include
    ./Vendor/GLM
    ./Vendor/Avir
    ./Vendor/JSON/single_include
    ./Vendor/ImGui
    ./Vendor/ImGui/backends
    ./Vendor/STB
)

set( GLFW_BUILD_DOCS OFF CACHE BOOL  "GLFW lib only" )
set( GLFW_INSTALL OFF CACHE BOOL  "GLFW lib only" )

add_subdirectory(./Vendor/GLFW)
add_subdirectory(./Vendor/Glad)
add_subdirectory(./Vendor/ImGuiBuilder)

if( MSVC )
    SET( CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} /ENTRY:mainCRTStartup" )
endif()

file(GLOB_RECURSE SOURCES
     ./WFCForge/Source/**.cpp
     ./WFCForge/Include/**.hpp
)

list(APPEND SOURCES ./Vendor/ImGui/backends/imgui_impl_glfw.cpp)
list(APPEND SOURCES ./Vendor/ImGui/backends/imgui_impl_opengl3.cpp)

add_executable(WFCForge ${SOURCES})

target_link_libraries(WFCForge
    glfw
    ${GLFW_LIBRARIES}
    opengl32
    Glad
    ImGui
)

target_compile_definitions(WFCForge
    PUBLIC _CRT_SECURE_NO_WARNINGS
)

if (CMAKE_CXX_COMPILER_VERSION VERSION_GREATER_EQUAL 19.12.25835)
    set(CMAKE_CXX20_STANDARD_COMPILE_OPTION "-std:c++latest")
    set(CMAKE_CXX20_EXTENSION_COMPILE_OPTION "-std:c++latest")
endif()

if( MSVC )
    if(${CMAKE_VERSION} VERSION_LESS "3.6.0") 
        message( "\n\t[ WARNING ]\n\n\tCMake version lower than 3.6.\n\n\t - Please update CMake and rerun; OR\n\t - Manually set 'GLFW-CMake-starter' as StartUp Project in Visual Studio.\n" )
    else()
        set_property( DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT WFCForge )
    endif()
endif()