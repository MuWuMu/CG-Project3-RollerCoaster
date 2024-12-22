cmake_minimum_required(VERSION 3.10)
project(roller-coaster)

set(SRC_DIR ${PROJECT_SOURCE_DIR}/src/)
set(RESOURCE_DIR ${PROJECT_SOURCE_DIR}/resources/)
set(INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include/)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/lib/)

include_directories(${INCLUDE_DIR})
include_directories(${LIB_DIR})

add_Definitions("-D_XKEYCHECK_H")
add_definitions(-DPROJECT_DIR="${PROJECT_SOURCE_DIR}")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(IMGUI_DIR ${INCLUDE_DIR}imgui/)

add_library(ImGUI
	${IMGUI_DIR}imconfig.h
	${IMGUI_DIR}imgui.cpp
	${IMGUI_DIR}imgui.h
	${IMGUI_DIR}imgui_demo.cpp
	${IMGUI_DIR}imgui_draw.cpp
	${IMGUI_DIR}imgui_internal.h
	${IMGUI_DIR}imgui_tables.cpp
	${IMGUI_DIR}imgui_widgets.cpp
	${IMGUI_DIR}imstb_rectpack.h
	${IMGUI_DIR}imstb_textedit.h
	${IMGUI_DIR}imstb_truetype.h
	${IMGUI_DIR}imgui_impl_glfw.h
	${IMGUI_DIR}imgui_impl_glfw.cpp
    ${IMGUI_DIR}imgui_impl_opengl3.h
    ${IMGUI_DIR}imgui_impl_opengl3.cpp
	${IMGUI_DIR}imgui_impl_opengl3_loader.h
)

add_library(glad STATIC ${SRC_DIR}/glad.c)

add_executable(roller-coaster 
	${SRC_DIR}main.cpp
	${SRC_DIR}glad.c
	${SRC_DIR}stb_image.cpp
	${SRC_DIR}Shader.cpp
	${SRC_DIR}Texture.cpp
	${SRC_DIR}Camera.cpp
	${SRC_DIR}wave.cpp
)

# Project output directory
set(PROJECT_OUTPUT_DIR ${CMAKE_BINARY_DIR}/$<CONFIG>)

# Copy shaders and resources to output directory
add_custom_command(
	TARGET roller-coaster POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_OUTPUT_DIR}/shaders
	COMMAND ${CMAKE_COMMAND} -E make_directory ${PROJECT_OUTPUT_DIR}/resources

	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${SRC_DIR}/shaders		${PROJECT_OUTPUT_DIR}/shaders

	COMMAND ${CMAKE_COMMAND} -E copy_directory
	${RESOURCE_DIR}			${PROJECT_OUTPUT_DIR}/resources
)

target_link_libraries(roller-coaster
	ImGUI
	glad
	${LIB_DIR}/glfw3.lib
	${LIB_DIR}/OpenGL32.lib
	gdi32
)
