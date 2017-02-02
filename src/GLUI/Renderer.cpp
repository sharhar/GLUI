#include <GLUI/GLUI.h>
#include <GLUIExt.h>

namespace glui {
	int Renderer::m_width = 0;
	int Renderer::m_height = 0;
	GLuint Renderer::quadVAO = 0;
	GLuint Renderer::quadVBOPosition = 0;
	GLuint Renderer::quadVBOTexCoord = 0;
	GLuint Renderer::vertShader = 0;
	GLuint Renderer::fragShader = 0;
	GLuint Renderer::shaderProgram = 0;
	GLuint Renderer::modeLoc = 0;
	GLuint Renderer::colorLoc = 0;
	GLuint Renderer::modelviewLoc = 0;
	GLuint Renderer::projectionLoc = 0;
	GLuint Renderer::texLoc = 0;
	float* Renderer::m_modelview = NULL;
    float* Renderer::m_projection = NULL;

	void Renderer::init(Window* window) {
		m_width = window->getWidth();
		m_height = window->getHeight();
		
		reinit();

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

		glGenVertexArrays(1, &quadVAO);
		glBindVertexArray(quadVAO);

		glGenBuffers(1, &quadVBOPosition);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBOPosition);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 2, verts, GL_STATIC_DRAW);
		glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &quadVBOTexCoord);
		glBindBuffer(GL_ARRAY_BUFFER, quadVBOTexCoord);
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

		vertShader = glCreateShader(GL_VERTEX_SHADER);
		fragShader = glCreateShader(GL_FRAGMENT_SHADER);
		GLchar* shadersource = (GLchar*)vertSource.c_str();
		glShaderSource(vertShader, 1, &shadersource, 0);
		shadersource = (GLchar*)fragSource.c_str();
		glShaderSource(fragShader, 1, &shadersource, 0);

		glCompileShader(vertShader);

		GLint compiled = 0;
		glGetShaderiv(vertShader, GL_COMPILE_STATUS, &compiled);
		if (compiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(vertShader, GL_INFO_LOG_LENGTH, &maxLength);

			GLchar* message = (GLchar*)malloc(sizeof(GLchar)*maxLength);
			glGetShaderInfoLog(vertShader, maxLength, &maxLength, message);

			std::cout << "Vertex Shader failed to compile:\n";
			std::cout << message << "\n";

			glDeleteShader(vertShader);
			return;
		}

		glCompileShader(fragShader);

		glGetShaderiv(fragShader, GL_COMPILE_STATUS, &compiled);
		if (compiled == GL_FALSE) {
			GLint maxLength = 0;
			glGetShaderiv(fragShader, GL_INFO_LOG_LENGTH, &maxLength);

			GLchar* message = (GLchar*)malloc(sizeof(GLchar)*maxLength);
			glGetShaderInfoLog(fragShader, maxLength, &maxLength, message);

			std::cout << "Fragment Shader failed to compile:\n";
			std::cout << message << "\n";

			glDeleteShader(fragShader);
			return;
		}

		shaderProgram = glCreateProgram();

		glAttachShader(shaderProgram, vertShader);
		glAttachShader(shaderProgram, fragShader);

		glBindAttribLocation(shaderProgram, 0, "position");
		glBindAttribLocation(shaderProgram, 1, "texcoord");

		glLinkProgram(shaderProgram);
		glValidateProgram(shaderProgram);

		modeLoc = glGetUniformLocation(shaderProgram, "mode");
		colorLoc = glGetUniformLocation(shaderProgram, "color");
		modelviewLoc = glGetUniformLocation(shaderProgram, "modelview");
		projectionLoc = glGetUniformLocation(shaderProgram, "projection");
		texLoc = glGetUniformLocation(shaderProgram, "tex");

		m_projection = new float[16];
		Utils::getOrthoMatrix(m_projection, 0, m_width, 0, m_height, -1, 1);

		glUseProgram(shaderProgram);

		glUniform1i(texLoc, 0);
		glUniformMatrix4fv(projectionLoc, 1, false, m_projection);

		glUseProgram(0);

		m_modelview = new float[16];
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
		glBindVertexArray(quadVAO);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glUseProgram(shaderProgram);
	}

	void Renderer::setUniforms(int mode, float* modelview) {
		glUniform1i(modeLoc, mode);
		glUniform3f(colorLoc, 0, 0, 0);
		glUniformMatrix4fv(modelviewLoc, 1, false, modelview);
	}

	void Renderer::setUniforms(int mode, float* modelview, glui::Color color) {
		glUniform1i(modeLoc, mode);
		glUniform3f(colorLoc, color.r, color.g, color.b);
		glUniformMatrix4fv(modelviewLoc, 1, false, modelview);
	}

	void Renderer::endDraw() {
		glUseProgram(0);
		
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(0);
		glBindVertexArray(0);
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

			Utils::getModelviewMatrix(m_modelview, x + w/2, y + h/2, w, h);
			Renderer::setUniforms(1, m_modelview, color);
			glDrawArrays(GL_TRIANGLES, 0, 6);

			xOff += (car->advance >> 6) * s;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
	}

	void Renderer::drawRect(float x, float y, float w, float h, Color color) {
		Utils::getModelviewMatrix(m_modelview, x + w / 2, y + h / 2, w, h);
		Renderer::setUniforms(0, m_modelview, color);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
    
    void Renderer::drawRect(float x, float y, float w, float h, GLuint tex) {
        glBindTexture(GL_TEXTURE_2D, tex);
        glActiveTexture(GL_TEXTURE0);
        
        Utils::getModelviewMatrix(m_modelview, x + w / 2, y + h / 2, w, h);
        Renderer::setUniforms(2, m_modelview);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
    void Renderer::drawRect(float x, float y, float w, float h, float r, Color color) {
        Utils::getModelviewMatrix(m_modelview, x, y, w, h, r);
        Renderer::setUniforms(0, m_modelview, color);
        glDrawArrays(GL_TRIANGLES, 0, 6);
    }
    
    void Renderer::setProjection(float left, float right, float bottom, float top, float near, float far) {
        Utils::getOrthoMatrix(m_projection, left, right, bottom, top, near, far);
        
        glUseProgram(shaderProgram);
        
        glUniformMatrix4fv(projectionLoc, 1, false, m_projection);
        
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
            
            Utils::getModelviewMatrix(m_modelview, x + w/2, y + h/2, w, h);
            //Renderer::setUniforms(1, m_modelview, color);
            glUniformMatrix4fv(modelviewLoc, 1, false, m_modelview);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            
            xOff += (car->advance >> 6) * s;
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);

    }
}
