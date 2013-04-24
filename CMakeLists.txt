cmake_minimum_required(VERSION 2.6)
project(Main)
#set(CMAKE_CXX_FLAGS "-Wall -std=c++11")
set(CMAKE_CXX_FLAGS "-std=c++11")

find_package(OpenGL REQUIRED)

add_subdirectory (external)

if(INCLUDE_DISTRIB)
	add_subdirectory(distrib)
endif(INCLUDE_DISTRIB)
list(APPEND CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/external/rpavlik-cmake-modules-1c73e35")
include(CreateLaunchers)

include_directories(
	external/AntTweakBar-1.15/include/
	external/glfw-2.7.6/include/
	external/glm-0.9.4.0/
	external/glew-1.9.0/include/
	.
)

set(ALL_LIBS
	${OPENGL_LIBRARY}
	GLFW_276
	GLEW_190
)

add_definitions(
	-DTW_STATIC
	-DTW_NO_LIB_PRAGMA
	-DTW_NO_DIRECT3D
	-DGLEW_STATIC
	-D_CRT_SECURE_NO_WARNINGS
)



set(BUILD_SHARED_LIBRARIES OFF)

### SDL
include(ExternalProject)
set(sdl_PREFIX "${CMAKE_CURRENT_BINARY_DIR}/sdl")
set(sdl_INSTALL_DIR "${CMAKE_CURRENT_BINARY_DIR}/sdl")
set(sdl_DOWNLOAD_DIR "${CMAKE_CURRENT_BINARY_DIR}/sdl/download")
#set(sdl_CMAKE_ARGS -D=off -DCMAKE_INSTALL_PREFIX=${sdl_INSTALL_DIR})
set(sdl_CMAKE_ARGS -DCMAKE_INSTALL_PREFIX=${sdl_INSTALL_DIR})

ExternalProject_Add(SDL
    PREFIX ${sdl_PREFIX}
    URL http://www.libsdl.org/tmp/SDL-2.0.zip
    INSTALL_DIR ${sdl_INSTALL_DIR}
    CMAKE_ARGS ${sdl_CMAKE_ARGS}
)

include_directories("${CMAKE_CURRENT_BINARY_DIR}/sdl/include")

add_executable(Main main.cc game_model.cc block.cc common/controls.cc common/controls.hpp common/shader.cpp common/shader.hpp)

target_link_libraries(Main ${sdl_PREFIX}/lib/libSDL2.a ${ALL_LIBS} pthread dl)

add_dependencies(Main SDL)
