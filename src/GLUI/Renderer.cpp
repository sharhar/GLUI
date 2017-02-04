#include <GLUI/GLUI.h>
#include <GLUIExt.h>

namespace glui {
	int Renderer::m_width = 0;
	int Renderer::m_height = 0;
	RendererGLData* Renderer::defaultRenderData = 0;
	RendererGLData* Renderer::currentRenderData = 0;

	void Renderer::init(Window* window) {
		m_width = window->getWidth();
		m_height = window->getHeight();
		
		reinit();

		defaultRenderData = createRenderData(m_width, m_height);
		setRenderData(defaultRenderData);
	}

	RendererGLData* Renderer::createRenderData(int width, int height) {
		RendererGLData* result = (RendererGLData*)malloc(sizeof(RendererGLData));

		GLfloat* verts = (GLfloat*)malloc(sizeof(GLfloat) * 6 * 2);

		verts[0] = -0.5f;
		verts[1] = -0.5f;

		verts[2] = 0.5f;
		verts[3] = -0.5f;

		verts[4] = 0.5f;
		verts[5] = 0.5f;

		verts[6] = -0.5f;
		verts[7] = -0.5f;

		verts[8] = -0.5f;
		verts[9] = 0.5f;

		verts[10] = 0.5f;
		verts[11] = 0.5f;

		GLfloat* texCoords = (GLfloat*)malloc(sizeof(GLfloat) * 6 * 2);

		texCoords[0] = 0;
		texCoords[1] = 1;

		texCoords[2] = 1;
		texCoords[3] = 1;

		texCoords[4] = 1;
		texCoords[5] = 0;

		texCoords[6] = 0;
		texCoords[7] = 1;

		texCoords[8] = 0;
		texCoords[9] = 0;

		texCoords[10] = 1;
		texCoords[11] = 0;

		glGenVertexArrays(1, &result->quadVAO);
		glBindVertexArray(result->quadVAO);

		glGenBuffers(1, &result->quadVBOPosition);
		glBindBuffer(GL_ARRAY_BUFFER, result->quadVBOPosition);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 2, verts, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &result->quadVBOTexCoord);
		glBindBuffer(GL_ARRAY_BUFFER, result->quadVBOTexCoord);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 2, texCoords, GL_STATIC_DRAW);
		glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glBindVertexArray(0);

		std::string vertSource = "";

		vertSource += "#version 330 core\n";

		vertSource += "in vec2 position;\n";
		vertSource += "in vec2 texcoord;\n";
		vertSource += "uniform mat4 modelview;\n";
		vertSource += "uniform mat4 projection;\n";

		vertSource += "out vec2 texcoord_out;\n";

		vertSource += "void main(void) {\n";
		vertSource += "gl_Position = projection * modelview * vec4(position.xy, 0, 1);\n";
		vertSource += "texcoord_out = texcoord;\n";
		vertSource += "}\n";

		std::string fragSource = "";

		fragSource += "#version 330 core\n";
		fragSource += "out vec4 out_color;\n";

		fragSource += "uniform int mode;\n";
		fragSource += "uniform vec3 color;\n";
		fragSource += "uniform sampler2D tex;\n";

		fragSource += "in vec2 texcoord_out;\n";

		fragSource += "void main(void) {\n";

		fragSource += "if(mode == 0) {\n";
		fragSource += "out_color = vec4(color.xyz, 1.0);\n";
		fragSource += "} else if(mode == 1) {\n";
		fragSource += "out_color = vec4(color.xyz, texture(tex, texcoord_out).r);\n";
		fragSource += "} else if(mode == 2) {\n";
		fragSource += "out_color = texture(tex, texcoord_out);\n";
		fragSource += "}\n";

		fragSource += "}\n";

		result->vertShader = glCreateShader(GL_VERTEX_SHADER);
		result->fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		GLchar* shadersource = (GLchar*)vertSource.c_str();
		glShaderSource(result->vertShader, 1, &shadersource, 0);
		shadersource = (GLchar*)fragSource.c_str();
		glShaderSource(result->fragShader, 1, &shadersource, 0);

		glCompileShader(result->vertShader);

		GLint compiled = 0;
		glGetShaderiv(result->vertShader, GL_COMPILE_STATUS, &compiled);
		if (compiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(result->vertShader, GL_INFO_LOG_LENGTH, &maxLength);

			GLchar* message = (GLchar*)malloc(sizeof(GLchar)*maxLength);
			glGetShaderInfoLog(result->vertShader, maxLength, &maxLength, message);

			std::cout << "Vertex Shader failed to compile:\n";
			std::cout << message << "\n";

			glDeleteShader(result->vertShader);
			return NULL;
		}

		glCompileShader(result->fragShader);

		glGetShaderiv(result->fragShader, GL_COMPILE_STATUS, &compiled);
		if (compiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(result->fragShader, GL_INFO_LOG_LENGTH, &maxLength);

			GLchar* message = (GLchar*)malloc(sizeof(GLchar)*maxLength);
			glGetShaderInfoLog(result->fragShader, maxLength, &maxLength, message);

			std::cout << "Fragment Shader failed to compile:\n";
			std::cout << message << "\n";

			glDeleteShader(result->fragShader);
			return NULL;
		}

		result->shaderProgram = glCreateProgram();

		glAttachShader(result->shaderProgram, result->vertShader);
		glAttachShader(result->shaderProgram, result->fragShader);

		glBindAttribLocation(result->shaderProgram, 0, "position");
		glBindAttribLocation(result->shaderProgram, 1, "texcoord");

		glLinkProgram(result->shaderProgram);
		glValidateProgram(result->shaderProgram);

		result->modeLoc = glGetUniformLocation(result->shaderProgram, "mode");
		result->colorLoc = glGetUniformLocation(result->shaderProgram, "color");
		result->modelviewLoc = glGetUniformLocation(result->shaderProgram, "modelview");
		result->projectionLoc = glGetUniformLocation(result->shaderProgram, "projection");
		result->texLoc = glGetUniformLocation(result->shaderProgram, "tex");

		result->projection = new float[16];
		Utils::getOrthoMatrix(result->projection, 0, width, 0, height, -1, 1);

		glUseProgram(result->shaderProgram);

		glUniform1i(result->texLoc, 0);
		glUniformMatrix4fv(result->projectionLoc, 1, false, result->projection);

		glUseProgram(0);

		result->modelview = new float[16];

		return result;
	}

	void Renderer::deleteRenderData(RendererGLData* renderData) {
		delete[] renderData->modelview;
		delete[] renderData->projection;

		glDetachShader(renderData->shaderProgram, renderData->vertShader);
		glDetachShader(renderData->shaderProgram, renderData->fragShader);
		glDeleteShader(renderData->vertShader);
		glDeleteShader(renderData->fragShader);
		glDeleteProgram(renderData->shaderProgram);

		glDeleteBuffers(1, &renderData->quadVBOPosition);
		glDeleteBuffers(1, &renderData->quadVBOTexCoord);
		glDeleteVertexArrays(1, &renderData->quadVAO);

		free(renderData);
	}

	void Renderer::reinit() {
		glViewport(0, 0, m_width, m_height);
		
		glDisable(GL_DEPTH_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	}

	void Renderer::clear(Color color) {
		glClearColor(color.r, color.g, color.b, 1);
		glClear(GL_COLOR_BUFFER_BIT);
	}

	void Renderer::beginDraw() {
		glBindVertexArray(currentRenderData->quadVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glUseProgram(currentRenderData->shaderProgram);
	}

	void Renderer::setUniforms(int mode, float* modelview) {
		glUniform1i(currentRenderData->modeLoc, mode);
		glUniform3f(currentRenderData->colorLoc, 0, 0, 0);
		glUniformMatrix4fv(currentRenderData->modelviewLoc, 1, false, modelview);
	}

	void Renderer::setUniforms(int mode, float* modelview, glui::Color color) {
		glUniform1i(currentRenderData->modeLoc, mode);
		glUniform3f(currentRenderData->colorLoc, color.r, color.g, color.b);
		glUniformMatrix4fv(currentRenderData->modelviewLoc, 1, false, modelview);
	}

	void Renderer::endDraw() {
		glUseProgram(0);
		
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
	}

	void Renderer::setRenderData(RendererGLData* renderData) {
		currentRenderData = renderData;
	}
	
	void Renderer::resetRenderData() {
		setRenderData(defaultRenderData);
	}

	void Renderer::drawString(const std::string& text, float posx, float posy, float scale, Font* font, Color color) {
		drawString(text, font->current, posx, posy, scale, font, color);
	}

	void Renderer::drawString(const std::string& text, int num, float posx, float posy, float scale, Font* font, Color color) {
		Character** chars = (Character**)font->chars[num];

		float xOff = 0;
		float yOff = 0;

		float s = scale/font->size;

		for (unsigned int i = 0; i < text.size();i++) {
			char c = text.at(i);

			if (c == '\n') {
				yOff += scale;
				xOff = 0;
				continue;
			}

			Character* car = chars[(int)c];

			GLuint tex = car->tex;
			
			glBindTexture(GL_TEXTURE_2D, tex);
			glActiveTexture(GL_TEXTURE0);

			float h = car->sizey * s;
			float w = car->sizex * s;
			
			float x = xOff + posx + car->offx * s;
			float y = posy - (car->sizey - car->offy) * s - yOff;

			Utils::getModelviewMatrix(currentRenderData->modelview, x + w/2, y + h/2, w, h);
			Renderer::setUniforms(1, currentRenderData->modelview, color);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			xOff += (car->advance >> 6) * s;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Renderer::drawRect(float x, float y, float w, float h, Color color) {
		Utils::getModelviewMatrix(currentRenderData->modelview, x + w / 2, y + h / 2, w, h);
		Renderer::setUniforms(0, currentRenderData->modelview, color);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
    
    void Renderer::drawRect(float x, float y, float w, float h, GLuint tex) {
        glBindTexture(GL_TEXTURE_2D, tex);
        glActiveTexture(GL_TEXTURE0);
        
        Utils::getModelviewMatrix(currentRenderData->modelview, x + w / 2, y + h / 2, w, h);
        Renderer::setUniforms(2, currentRenderData->modelview);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
    void Renderer::drawRect(float x, float y, float w, float h, float r, Color color) {
        Utils::getModelviewMatrix(currentRenderData->modelview, x, y, w, h, r);
        Renderer::setUniforms(0, currentRenderData->modelview, color);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
    void Renderer::setProjection(float left, float right, float bottom, float top, float near, float far) {
        Utils::getOrthoMatrix(currentRenderData->projection, left, right, bottom, top, near, far);
        
        glUseProgram(currentRenderData->shaderProgram);
        glUniformMatrix4fv(currentRenderData->projectionLoc, 1, false, currentRenderData->projection);
        glUseProgram(0);
    }
    
    void Renderer::drawStringCustom(const std::string& text, GLuint modelviewLoc, float posx, float posy, float scale, Font* font, Color color) {
        Character** chars = (Character**)font->chars[font->current];
        
        float xOff = 0;
        float yOff = 0;
        
        float s = scale/font->size;
        
        for (unsigned int i = 0; i < text.size();i++) {
            char c = text.at(i);
            
            if (c == '\n') {
                yOff += scale;
                xOff = 0;
                continue;
            }
            
            Character* car = chars[(int)c];
            
            GLuint tex = car->tex;
            
            glBindTexture(GL_TEXTURE_2D, tex);
            glActiveTexture(GL_TEXTURE0);
            
            float h = car->sizey * s;
            float w = car->sizex * s;
            
            float x = xOff + posx + car->offx * s;
            float y = posy - (car->sizey - car->offy) * s - yOff;
            
            Utils::getModelviewMatrix(currentRenderData->modelview, x + w/2, y + h/2, w, h);
            glUniformMatrix4fv(modelviewLoc, 1, false, currentRenderData->modelview);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            xOff += (car->advance >> 6) * s;
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);

    }
}
