cmake_minimum_required(VERSION 2.8.11)

file(GLOB_RECURSE GLUI_HEADERS "${CMAKE_SOURCE_DIR}/include/*.h")
file(GLOB_RECURSE GLUI_SRC *.cpp)

add_library(GLUI ${GLUI_HEADERS}  ${GLUI_SRC})

target_include_directories (GLUI PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries (GLUI LINK_PUBLIC ${OPENGL_LIBRARIES})
