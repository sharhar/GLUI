# GLUI (OpenGL User Interface)

### General info
This library was made so that there would be a cross platform UI library the uses and integrates OpenGL. Right now it is in very early stages of deveopment and will have much more (and existent) documentation and many, many, more features.

### How to compile
This repo uses [cmake](https://cmake.org/) to compile to multiple platforms. When the library is built three static libraries will be put into the libs folder whee you built the library to. These libraries are all required for using GLUI. a GLUI-Test exceutable will be put into the bin folder if BUILD_GLUI-TEST is enabled in cmake. 

### Documentation
Currently there is no documentation of the library. The src/GLUI-Test directory has an example program that uses the library and cmake. for now this is the only thing closeto documentation for people who want to learn how to use the library. I will work on adding documentation when the core features of the library are finished.
