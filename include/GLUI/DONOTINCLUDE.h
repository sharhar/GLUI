#pragma once

#include <iostream>
#include <GLFW/glfw3.h>
#include <ft2build.h>
#include FT_FREETYPE_H

typedef struct GLUIData {
	FT_Library ft;
	GLuint texVAO, texVBO;
	GLint texColorLoc;
} GLUIData;

typedef struct Character {
	GLuint tex;
	float sizex;
	float sizey;
	float offx;
	float offy;
	GLuint advance;
} Character;