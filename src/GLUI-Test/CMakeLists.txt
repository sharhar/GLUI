cmake_minimum_required(VERSION 2.8.11)

file(GLOB_RECURSE GLUI_TEST_SRC *.cpp)

add_executable(GLUI-Test  ${GLUI_TEST_SRC})

target_link_libraries (GLUI-Test LINK_PUBLIC ${OPENGL_LIBRARIES})
target_link_libraries (GLUI-Test glfw ${GLFW_LIBRARIES})
target_link_libraries (GLUI-Test LINK_PUBLIC GLUI)