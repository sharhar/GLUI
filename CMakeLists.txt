cmake_minimum_required(VERSION 2.8.11)

project(GLUI)

#all the variables are now being set here

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(OpenGL REQUIRED)

include_directories(
	"${CMAKE_SOURCE_DIR}/ext/glfw/include"
	"${CMAKE_SOURCE_DIR}/ext/freetype-2.6/include"
	"${CMAKE_SOURCE_DIR}/ext/glew-1.13.0/include"
	"${CMAKE_SOURCE_DIR}/ext/freetype-2.6/devel"
	"${CMAKE_SOURCE_DIR}/ext/freetype-gl"
	"${CMAKE_SOURCE_DIR}/include"
)

add_subdirectory("${CMAKE_SOURCE_DIR}/ext/glfw")
add_subdirectory("${CMAKE_SOURCE_DIR}/ext/freetype-2.6")
add_subdirectory("${CMAKE_SOURCE_DIR}/ext/freetype-gl")
add_subdirectory("${CMAKE_SOURCE_DIR}/ext/glew-1.13.0/build/cmake")

add_subdirectory("${CMAKE_SOURCE_DIR}/src/GLUI")
add_subdirectory("${CMAKE_SOURCE_DIR}/src/GLUI-Test")