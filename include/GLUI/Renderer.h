#pragma once

#include <GLUI/Window.h>
#include <GLUI/Utils.h>
#include <string>

namespace glui {
	class Renderer {
	private:
		static int m_width, m_height;

		static GLuint quadVAO, quadVBOPosition, quadVBOTexCoord;
		static GLuint vertShader, fragShader, shaderProgram;
		static GLuint modeLoc, colorLoc, modelviewLoc, projectionLoc, texLoc;
        static float* m_modelview;
        static float* m_projection;
	public:
		static void init(Window* window);
		static void reinit();
		static void clear(Color color);

		static void beginDraw();
		static void setUniforms(int mode, float* modelview);
		static void setUniforms(int mode, float* modelview, glui::Color color);
		static void endDraw();

		static void drawString(const std::string& text, float posx, float posy, float scale, Font* font, Color color);
		static void drawString(const std::string& text, int num, float posx, float posy, float scale, Font* font, Color color);
		static void drawRect(float x, float y, float w, float h, Color color);
        static void drawRect(float x, float y, float w, float h, GLuint tex);
        static void drawRect(float x, float y, float w, float h, float r, Color color);
        static void setProjection(float left, float right, float bottom, float top, float near, float far);
	};
}
