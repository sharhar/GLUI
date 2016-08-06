cmake_minimum_required(VERSION 2.8.11)

project(GLUI)

#all the variables are now being set here

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(BUILD_GLUI-TEST ON)

find_package(OpenGL REQUIRED)

if(NOT MSVC)

	include(CheckCXXCompilerFlag)
	CHECK_CXX_COMPILER_FLAG("-std=c++11" COMPILER_SUPPORTS_CXX11)
	CHECK_CXX_COMPILER_FLAG("-std=c++0x" COMPILER_SUPPORTS_CXX0X)
	if(COMPILER_SUPPORTS_CXX11)
		message(STATUS "Found CXX11 support in ${CMAKE_CXX_COMPILER}")
	    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
	elseif(COMPILER_SUPPORTS_CXX0X)
		message(STATUS "Found CXX0X support in ${CMAKE_CXX_COMPILER}")
	    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")
	else()
		message(FATAL_ERROR "The compiler ${CMAKE_CXX_COMPILER} has no C++11 support. Please use a different C++ compiler.")
	endif()

endif()

if(NOT EXISTS "${CMAKE_SOURCE_DIR}/ext/glcorearb/glcorearb.h")
	message("Did not find glcorearb.h...")
	message("Downloading glcorearb.h...")
	file(DOWNLOAD "https://www.opengl.org/registry/api/GL/glcorearb.h" "${CMAKE_SOURCE_DIR}/ext/glcorearb/glcorearb.h")
else()
	message("Found glcorearb.h...")
endif()

include_directories(
	"${CMAKE_SOURCE_DIR}/ext/glfw/include"
	"${CMAKE_SOURCE_DIR}/ext/freetype-2.6/include"
	"${CMAKE_SOURCE_DIR}/ext/freetype-2.6/devel"
	"${CMAKE_SOURCE_DIR}/ext/glcorearb"
	"${CMAKE_SOURCE_DIR}/ext/GLUIExt"
	"${CMAKE_SOURCE_DIR}/include"
)

add_subdirectory("${CMAKE_SOURCE_DIR}/ext/glfw")
add_subdirectory("${CMAKE_SOURCE_DIR}/ext/freetype-2.6")

add_subdirectory("${CMAKE_SOURCE_DIR}/src/GLUI")

if(BUILD_GLUI-TEST)
	add_subdirectory("${CMAKE_SOURCE_DIR}/src/GLUI-Test")
	
	file(COPY "${CMAKE_SOURCE_DIR}/arial.ttf" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()
