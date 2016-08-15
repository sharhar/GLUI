cmake_minimum_required(VERSION 2.8.11)

project(GLUI)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

set(BUILD_GLUI-TEST ON)

set(EXT_EXTRACT_DIR "${CMAKE_SOURCE_DIR}/ext")

set(GLFW_SOURCE_NAME "glfw-3.2")
set(GLFW_URL "http://downloads.sourceforge.net/project/glfw/glfw/3.2/${GLFW_SOURCE_NAME}.tar.gz?r=https%3A%2F%2Fsourceforge.net%2Fprojects%2Fglfw%2Ffiles%2Fglfw%2F3.2%2F&ts=1470492725&use_mirror=heanet")
set(GLFW_DOWNLOAD_PATH "${CMAKE_SOURCE_DIR}/${GLFW_SOURCE_NAME}.tar.gz")
set(GLFW_SOURCE_DIR "${EXT_EXTRACT_DIR}/${GLFW_SOURCE_NAME}")

set(FT_SOURCE_NAME "freetype-2.6")
set(FT_URL "http://nongnu.askapache.com//freetype/${FT_SOURCE_NAME}.tar.gz")
set(FT_DOWNLOAD_PATH "${CMAKE_SOURCE_DIR}/${FT_SOURCE_NAME}.tar.gz")
set(FT_SOURCE_DIR "${EXT_EXTRACT_DIR}/${FT_SOURCE_NAME}")

#http://nongnu.askapache.com//freetype/freetype-2.6.tar.gz

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

if(NOT EXISTS ${GLFW_SOURCE_DIR})
	message(STATUS "GLFW source not found. Fixing..")
	#First we check if the .tar.gz is downloaded. There may be chance it is already downloaded and not extracted
	if (NOT EXISTS "${GLFW_DOWNLOAD_PATH}")
		message(STATUS "Downloading GLFW source...")
		file(DOWNLOAD "${GLFW_URL}" "${GLFW_DOWNLOAD_PATH}")
	else()
		message(STATUS "GLFW source found in tar.gz")
	endif()

	message(STATUS "Extracting GLFW source...")
	#Next we extract the .tar.gz. This works on Windows as well
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E tar xvf ${GLFW_DOWNLOAD_PATH}
		WORKING_DIRECTORY ${EXT_EXTRACT_DIR}
	)
		
	#If the extraction fails, go to here.
	if(NOT EXISTS ${GLFW_SOURCE_DIR})
		message(FATAL_ERROR "Failed to extract GLFW source! Please retry or file a bug report.")
	else()
		message(STATUS "GLFW extracted.")
	endif()
		
	#Remove the extracted tar. We dont need the original anymore
	file(REMOVE ${GLFW_DOWNLOAD_PATH})
	message(STATUS "Deleted temporary tar")
	message(STATUS "Done adding ${GLFW_SOURCE_NAME}")
else()
	message(STATUS "GLFW found.")
endif()

if(NOT EXISTS ${FT_SOURCE_DIR})
	message(STATUS "FreeType source not found. Fixing..")
	#First we check if the .tar.gz is downloaded. There may be chance it is already downloaded and not extracted
	if (NOT EXISTS "${FT_DOWNLOAD_PATH}")
		message(STATUS "Downloading FreeType source...")
		file(DOWNLOAD "${FT_URL}" "${FT_DOWNLOAD_PATH}")
	else()
		message(STATUS "FreeType source found in tar.gz")
	endif()

	message(STATUS "Extracting FreeType source...")
	#Next we extract the .tar.gz. This works on Windows as well
	execute_process(
		COMMAND ${CMAKE_COMMAND} -E tar xvf ${FT_DOWNLOAD_PATH}
		WORKING_DIRECTORY ${EXT_EXTRACT_DIR}
	)
		
	#If the extraction fails, go to here.
	if(NOT EXISTS ${FT_SOURCE_DIR})
		message(FATAL_ERROR "Failed to extract FreeType source! Please retry or file a bug report.")
	else()
		message(STATUS "FreeType extracted.")
	endif()
		
	#Remove the extracted tar. We dont need the original anymore
	file(REMOVE ${FT_DOWNLOAD_PATH})
	message(STATUS "Deleted temporary tar")
	message(STATUS "Done adding ${FT_SOURCE_NAME}")
else()
	message(STATUS "FreeType found.")
endif()


include_directories(
	"${GLFW_SOURCE_DIR}/include"
	"${FT_SOURCE_DIR}/include"
	"${FT_SOURCE_DIR}/ext/freetype-2.6/devel"
	"${CMAKE_SOURCE_DIR}/ext/glcorearb"
	"${CMAKE_SOURCE_DIR}/ext/GLUIExt"
	"${CMAKE_SOURCE_DIR}/include"
)


option(GLFW_BUILD_EXAMPLES "Build the GLFW example programs" OFF)

option(GLFW_BUILD_TESTS "Build the GLFW test programs" OFF)

option(GLFW_BUILD_DOCS "Build the GLFW documentation" OFF)

add_subdirectory("${GLFW_SOURCE_DIR}")
add_subdirectory("${CMAKE_SOURCE_DIR}/ext/freetype-2.6")

add_subdirectory("${CMAKE_SOURCE_DIR}/src/GLUI")

if(BUILD_GLUI-TEST)
	add_subdirectory("${CMAKE_SOURCE_DIR}/src/GLUI-Test")
endif()
