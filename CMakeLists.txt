cmake_minimum_required(VERSION 3.1)

project(OpenGL)

set(CMAKE_CXX_STANDARD 11)

configure_file(CMakeProjectMacros.h.in Project-macros/CMakeProjectMacros.h)

add_executable(OpenGL Source/main.cpp Source/glad.c Source/Camera.h Source/Camera.cpp Source/CameraController.h Source/CameraController.cpp  OpenGLTypes/Shader.cpp OpenGLTypes/Shader.h )

include_directories( Dependencies/Include  "${PROJECT_SOURCE_DIR}/Source" "${PROJECT_BINARY_DIR}/Project-macros" OpenGLTypes)

find_library(GLFW_LIBRARY glfw3 glfw3.lib "${PROJECT_SOURCE_DIR}/Dependencies/Lib")

target_link_libraries(OpenGL LINK_PUBLIC ${GLFW_LIBRARY} opengl32.lib)



