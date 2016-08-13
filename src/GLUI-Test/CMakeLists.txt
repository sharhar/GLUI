cmake_minimum_required(VERSION 2.8.11)

file(GLOB_RECURSE GLUI_TEST_SRC *.cpp)

if(APPLE)
	add_executable(
		GLUI-Test 
		MACOSX_BUNDLE 
		${GLUI_TEST_SRC}
		"${CMAKE_SOURCE_DIR}/arial.ttf"
		)
	
	SET_SOURCE_FILES_PROPERTIES(
	  "${CMAKE_SOURCE_DIR}/arial.ttf"
	  PROPERTIES
	  MACOSX_PACKAGE_LOCATION Resources
	  )
else()
	add_executable(GLUI-Test ${GLUI_TEST_SRC})
	file(COPY "${CMAKE_SOURCE_DIR}/arial.ttf" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
endif()

target_link_libraries (GLUI-Test LINK_PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries (GLUI-Test LINK_PUBLIC GLUI)
target_link_libraries (GLUI-Test glfw ${GLFW_LIBRARIES})
target_link_libraries (GLUI-Test LINK_PUBLIC freetype)