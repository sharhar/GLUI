#include <GLUI/GLUI.h>
#include <GLUIExt.h>

namespace glui {

	Font::Font(std::string path, float a_size) {
		GLUIData* data = (GLUIData*)GLUI::data;

		FT_Face face;

		if (FT_New_Face(data->ft, path.c_str(), 0, &face)) {
			fprintf(stderr, "Could not open font\n");
			m_inited = false;
			return;
		}

		size = a_size;

		FT_Set_Pixel_Sizes(face, 0, (int)size);

		m_face = face;

		loadGL(0);
		current = 0;

		m_inited = true;
	}

	void Font::loadGL(int num) {
		if (chars.size() > num) {
			del(num);
		}

		FT_Face face = (FT_Face)m_face;

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

			glPixelTransferf(GL_RED_BIAS, 1);
			glPixelTransferf(GL_GREEN_BIAS, 1);
			glPixelTransferf(GL_BLUE_BIAS, 1);

			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RGBA,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_ALPHA,
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

			t_chars[c] = car;
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
	}

	bool Font::inited() {
		return m_inited;
	}

	GLUIObject::GLUIObject(Rectangle bounds, Layout* layout) {
		m_bounds = bounds;
		m_layout = layout;
	}

	void GLUIObject::setPos(Vector2f pos) {
		m_bounds.x = pos.x;
		m_bounds.y = pos.y;
	}
	
	void GLUIObject::setSize(Vector2f size) {
		m_bounds.w = size.x;
		m_bounds.h = size.y;
	}
}