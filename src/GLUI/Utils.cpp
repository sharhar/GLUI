#include <GLUI/GLUI.h>
#include <GLUIExt.h>
#include <math.h>

#define DEG_TO_RAD 0.0174532925199

namespace glui {

	FontFace::FontFace(std::string path) {
		GLUIData* data = (GLUIData*)GLUI::data;

		FT_Face face;

		if (FT_New_Face(data->ft, path.c_str(), 0, &face)) {
			fprintf(stderr, "Could not open font\n");
			m_inited = false;
			return;
		}

		m_face = face;

		m_inited = true;
	}

	bool FontFace::inited() {
		return m_inited;
	}

	void* FontFace::getFace() {
		return m_face;
	}

	Font::Font(FontFace* face, int fontSize) {
		m_face = face;
		size = fontSize;
		loadGL(0);
		current = 0;
	}

	void Font::loadGL(int num) {
		if (chars.size() > num) {
			del(num);
		}

		FT_Face face = (FT_Face)m_face->getFace();

		FT_Set_Pixel_Sizes(face, 0, size);

		Character** t_chars = new Character*[127];

		for (char c = 0; c < 127; c++) {
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
				std::cout << "Couldn't load '" << c << "'\n";
				continue;
			}

			GLuint tex;
			glGenTextures(1, &tex);
			glBindTexture(GL_TEXTURE_2D, tex);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGB,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
				);

			Character* car = (Character*)malloc(sizeof(Character));

			car->tex = tex;

			car->sizex = face->glyph->bitmap.width;
			car->sizey = face->glyph->bitmap.rows;
			car->offx = face->glyph->bitmap_left;
			car->offy = face->glyph->bitmap_top;
			car->advance = face->glyph->advance.x;

			t_chars[(int)c] = car;
		}

		if (chars.size() <= num) {
			chars.push_back((void*)t_chars);
		} else {
			chars[num] = (void*)t_chars;
		}
	}

	void Font::del(int num) {
		Character** t_chars = (Character**)chars[num];
		for (int i = 0; i < 127;i++) {
			free(t_chars[i]);
		}

		free(t_chars);

		chars.pop_back();
	}

	GLUIObject::GLUIObject(Rectangle bounds, Layout* layout, int windowID) {
		m_bounds = bounds;
		m_layout = layout;
		m_windowID = windowID;
	}

	void GLUIObject::setPos(Vector2f pos) {
		m_bounds.x = pos.x;
		m_bounds.y = pos.y;
	}
	
	void GLUIObject::setSize(Vector2f size) {
		m_bounds.w = size.x;
		m_bounds.h = size.y;
	}

	void GLUIObject::poll() {
		if (input::InputData::windowID == m_windowID) {
			pollFunction();
		}
	}

	void Utils::getModelviewMatrix(float* vals, float x, float y, float w, float h) {
		vals[0] = w;
		vals[1] = 0;
		vals[2] = 0;
		vals[3] = 0;
		
		vals[4] = 0;
		vals[5] = h;
		vals[6] = 0;
		vals[7] = 0;
		
		vals[8] = 0;
		vals[9] = 0;
		vals[10] = 1;
		vals[11] = 0;

		vals[12] = x;
		vals[13] = y;
		vals[14] = 0;
		vals[15] = 1;
	}
    
    void Utils::getModelviewMatrix(float* vals, float x, float y, float w, float h, float r) {
        float c = cos(r*DEG_TO_RAD);
        float s = sin(r*DEG_TO_RAD);
        
        vals[0] = w*c;
        vals[1] = w*s;
        vals[2] = 0;
        vals[3] = 0;
        
        vals[4] = -h*s;
        vals[5] = h*c;
        vals[6] = 0;
        vals[7] = 0;
        
        vals[8] = 0;
        vals[9] = 0;
        vals[10] = 1;
        vals[11] = 0;
        
        vals[12] = x;
        vals[13] = y;
        vals[14] = 0;
        vals[15] = 1;
    }

	void Utils::getOrthoMatrix(float* vals, float left, float right, float bottom, float top, float near, float far) {
		vals[0] = 2.0f / (right - left);
		vals[1] = 0;
		vals[2] = 0;
		vals[3] = 0;

		vals[4] = 0;
		vals[5] = 2.0f / (top - bottom);
		vals[6] = 0;
		vals[7] = 0;

		vals[8] = 0;
		vals[9] = 0;
		vals[10] = -2.0f / (far - near);
		vals[11] = 0;

		vals[12] = -(right + left) / (right - left);
		vals[13] = -(top + bottom) / (top - bottom);
		vals[14] = -(far + near) / (far - near);
		vals[15] = 1;
	}
}
