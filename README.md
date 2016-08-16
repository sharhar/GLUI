# GLUI (OpenGL User Interface)

Travis-ci<br> ![picture alt](https://travis-ci.org/sharhar/GLUI.svg?branch=master "Travis.ci build status")
### General info
This library was made so that there would be a cross platform UI library the uses and integrates OpenGL. Right now it is in very early stages of deveopment and will have much more (and existent) documentation and many, many, more features.

### How to compile
This repo uses [cmake](https://cmake.org/) to compile to multiple platforms. After the prject is compiled, three static libraries will be put into the libs folder where you built the library to. These libraries are all required for using GLUI. A GLUI-Test exceutable will be put into the bin folder if BUILD_GLUI_TEST is enabled in cmake (by default it is). 

### Documentation
Currently there is no documentation of the library. The src/GLUI-Test directory has an example program that uses the library and cmake. for now this is the only thing closeto documentation for people who want to learn how to use the library. I will work on adding documentation when the core features of the library are finished.

### Dependencies
All dependecies are downloaded when cmake is run. <br> 
These dependecies are:
* [GLFW] (http://www.glfw.org/) 3.2
* [FreeType] (https://www.freetype.org/) 2.6
* [glcorearb.h] (https://www.opengl.org/registry/api/GL/glcorearb.h)



