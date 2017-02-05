#pragma once

#include <GLUI/Window.h>
#include <GLUI/Utils.h>
#include <string>

namespace glui {
	typedef struct RendererGLData {
		GLuint quadVAO, quadVBOPosition, quadVBOTexCoord;
		GLuint vertShader, fragShader, shaderProgram;
		GLuint modeLoc, colorLoc, modelviewLoc, projectionLoc, texLoc;
		float* modelview;
		float* projection;
	} RendererGLData;
	
	class Renderer {
	private:
		static int m_width, m_height;

		static RendererGLData* defaultRenderData;
	public:
		static void init(Window* window);
		static RendererGLData* createRenderData(int width, int height);
		static void deleteRenderData(RendererGLData* renderData);
		static void reinit();
		static void clear(Color color);

		static void beginDraw();
		static void setUniforms(int mode, float* modelview);
		static void setUniforms(int mode, float* modelview, glui::Color color);
		static void endDraw();

		static void drawString(std::string text, float posx, float posy, float scale, Font* font, Color color);
		static void drawString(std::string text, int num, float posx, float posy, float scale, Font* font, Color color);
		static void drawStringCustom(std::string text, GLuint modelviewLoc, float posx, float posy, float scale, Font* font, Color color);

		static void drawRect(float x, float y, float w, float h, Color color);
        static void drawRect(float x, float y, float w, float h, GLuint tex);
        static void drawRect(float x, float y, float w, float h, float r, Color color);
        static void setProjection(float left, float right, float bottom, float top, float near, float far);


		static void beginDraw(RendererGLData* renderData);
		static void setUniforms(RendererGLData* renderData, int mode, float* modelview, glui::Color color);

		static void drawString(RendererGLData* renderData, std::string text, int num, float posx, float posy, float scale, Font* font, Color color);
		static void drawStringCustom(RendererGLData* renderData, std::string text, GLuint modelviewLoc, float posx, float posy, float scale, Font* font, Color color);

		static void drawRect(RendererGLData* renderData, float x, float y, float w, float h, Color color);
		static void drawRect(RendererGLData* renderData, float x, float y, float w, float h, GLuint tex);
		static void drawRect(RendererGLData* renderData, float x, float y, float w, float h, float r, Color color);
		static void setProjection(RendererGLData* renderData, float left, float right, float bottom, float top, float near, float far);
	};
}
