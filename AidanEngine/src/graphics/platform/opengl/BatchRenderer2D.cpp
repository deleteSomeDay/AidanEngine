#include "BatchRenderer2D.h"
namespace engine {
	namespace graphics {

		
		BatchRenderer2D::BatchRenderer2D()
		{
			init();
		}

		BatchRenderer2D::~BatchRenderer2D()
		{
			delete m_IBO;
			glDeleteBuffers(1, &m_VBO);
		}

		void BatchRenderer2D::init()
		{
			glGenVertexArrays(1, &m_VAO);

			glGenBuffers(1, &m_VBO);
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

			glBindVertexArray(m_VAO);

			glEnableVertexAttribArray(SHADER_VERTEX_INDEX);
			glEnableVertexAttribArray(SHADER_UV_INDEX);
			glEnableVertexAttribArray(SHADER_TID_INDEX);
			glEnableVertexAttribArray(SHADER_COLOR_INDEX);

			glVertexAttribPointer(SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)0);
			glVertexAttribPointer(SHADER_UV_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::uv)));
			glVertexAttribPointer(SHADER_TID_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::tid)));
			glVertexAttribPointer(SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (const GLvoid*)(offsetof(VertexData, VertexData::color)));

			glBindBuffer(GL_ARRAY_BUFFER, 0);

			GLushort indices[RENDERER_INDICES_SIZE];

			int offset = 0;
			for (int i = 0; i < RENDERER_INDICES_SIZE; i += 6)
			{
				indices[i] = offset + 0;
				indices[i + 1] = offset + 1;
				indices[i + 2] = offset + 2;

				indices[i + 3] = offset + 2;
				indices[i + 4] = offset + 3;
				indices[i + 5] = offset + 0;

				offset += 4;
			}

			m_IBO = new IndexBuffer(indices, RENDERER_INDICES_SIZE);

			glBindVertexArray(0);

			FT_Library ft;
			if (FT_Init_FreeType(&ft))
			{
				std::cout << "Error: Freetype lib couldnt init" << std::endl;
			}

			FT_Face face;
			if (FT_New_Face(ft, "fonts/times.ttf", 0, &face))
				std::cout << "Error: Freetype: Failed to load font" << std::endl;

			FT_Set_Pixel_Sizes(face, 0, 48);
		
			glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

			for (GLubyte c = 0; c < 128; c++)
			{

				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					std::cout << "Error: Freetype: Failed to load Glyph" << std::endl;
					continue;
				}

				unsigned int tid;
				glGenTextures(1, &tid);
				glBindTexture(GL_TEXTURE_2D, tid);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_INTENSITY, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_LUMINANCE , GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);
				
				Character character = {
					tid,
					maths::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					maths::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					face->glyph->advance.x
				};
				m_CharCache.insert(std::pair<GLchar, Character>(c, character));

				glBindTexture(GL_TEXTURE_2D, 0);
			}
			FT_Done_Face(face);
			FT_Done_FreeType(ft);

			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			
			glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
		}
		void BatchRenderer2D::begin()
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
			m_Buffer = (VertexData*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
			std::vector<unsigned int> a;
			m_TextureSlots = a;
		}
		void BatchRenderer2D::submit(const Renderable2D* renderable)
		{
			const maths::vec3& position = renderable->getPosition();
			const maths::vec2& size = renderable->getSize();
			const maths::vec4& color = renderable->getColor();
			const std::vector<maths::vec2>& uv = renderable->getUV();
			const unsigned int tid = renderable->getTID();

			unsigned int c = 0;
			float ts = 0.0f;
			if (tid > 0)
			{
				bool found = false;
			
					for(int i = 0; i < m_TextureSlots.size(); i++)
					{
						if (m_TextureSlots[i] == tid)
						{
							ts = (float)(i + 1);
							found = true;
							break;
						}
					}
					if (!found)
					{
						if (m_TextureSlots.size() >= 32)
						{
							end();
							flush();
							begin();
						}
						m_TextureSlots.push_back(tid);
						ts = (float)(m_TextureSlots.size());
					}
			}
			
				int r = color.x * 255.0f;
				int g = color.y * 255.0f;
				int b = color.w * 255.0f;
				int a = color.z * 255.0f;

				c = a << 24 | b << 16 | g << 8 | r;
			

			m_Buffer->vertex = *m_TransformationBack * position;
			m_Buffer->uv = uv[0];
			m_Buffer->tid = ts;
			m_Buffer->color = c;
			m_Buffer++;

			m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x, position.y + size.y, position.z);
			m_Buffer->uv = uv[1];
			m_Buffer->tid = ts;
			m_Buffer->color = c;
			m_Buffer++;

			m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x + size.x, position.y + size.y, position.z);
			m_Buffer->uv = uv[2];
			m_Buffer->tid = ts;
			m_Buffer->color = c;
			m_Buffer++;

			m_Buffer->vertex = *m_TransformationBack * maths::vec3(position.x + size.x, position.y, position.z);
			m_Buffer->uv = uv[3];
			m_Buffer->tid = ts;
			m_Buffer->color = c;
			m_Buffer++;

			m_IndexCount += 6;
		}

		void BatchRenderer2D::end()
		{
			glUnmapBuffer(GL_ARRAY_BUFFER);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
		void BatchRenderer2D::drawString(const std::string& string, const maths::vec3& position, const maths::vec4 color, float scale)
		{
			int r = color.x * 255.0f;
			int g = color.y * 255.0f;
			int b = color.w * 255.0f;
			int a = color.z * 255.0f;

			unsigned int col = a << 24 | b << 16 | g << 8 | r;

			float x = position.x;
			float ts = 0.0f;

			bool found = false;
			for (std::string::const_iterator c = string.begin(); c != string.end(); c++)
			{
				Character ch = m_CharCache[*c];

				GLfloat xpos = x + ch.bearing.x * scale;
				GLfloat ypos = position.y - (ch.size.y - ch.bearing.y) * scale;

				GLfloat w = ch.size.x * scale;
				GLfloat h = ch.size.y * scale;
			
				x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64)

				for (int i = 0; i < m_TextureSlots.size(); i++)
				{
					if (m_TextureSlots[i] == m_CharCache[*c].tid)
					{
						ts = (float)(i + 1);
						found = true;
						break;
					}
				}
				if (!found)
				{
					if (m_TextureSlots.size() >= 32)
					{
						end();
						flush();
						begin();
					}
					m_TextureSlots.push_back(m_CharCache[*c].tid);
					ts = (float)(m_TextureSlots.size());
				}
					
				m_Buffer->vertex = *m_TransformationBack * maths::vec3(xpos, ypos + h, 0);
				m_Buffer->uv = maths::vec2(0, 0);
				m_Buffer->tid = ts;
				m_Buffer->color = col;
				m_Buffer++;

				m_Buffer->vertex = *m_TransformationBack * maths::vec3(xpos, ypos, 0);
				m_Buffer->uv = maths::vec2(0, 1);
				m_Buffer->tid = ts;
				m_Buffer->color = col;
				m_Buffer++;

				m_Buffer->vertex = *m_TransformationBack * maths::vec3(xpos + w, ypos, 0);
				m_Buffer->uv = maths::vec2(1, 1);
				m_Buffer->tid = ts;
				m_Buffer->color = col;
				m_Buffer++;

				m_Buffer->vertex = *m_TransformationBack * maths::vec3(xpos + w, ypos + h, 0);
				m_Buffer->uv = maths::vec2(1, 0);
				m_Buffer->tid = ts;
				m_Buffer->color = col;
				m_Buffer++;

				m_IndexCount += 6;

				found = false;
			}
		}
		void BatchRenderer2D::changeFont(const char* filename)
		{
			m_CharCache.clear();

			FT_Library ft;
			if (FT_Init_FreeType(&ft))
			{
				std::cout << "Error: Freetype lib couldnt init" << std::endl;
			}
			
			FT_Face face;
			if (FT_New_Face(ft, filename, 0, &face))
				std::cout << "Error: Freetype: Failed to load font" << std::endl;

			FT_Set_Pixel_Sizes(face, 0, 48);

			for (GLubyte c = 0; c < 128; c++)
			{

				if (FT_Load_Char(face, c, FT_LOAD_RENDER))
				{
					std::cout << "Error: Freetype: Failed to load Glyph" << std::endl;
					continue;
				}

				unsigned int tid;
				glGenTextures(1, &tid);
				glBindTexture(GL_TEXTURE_2D, tid);

				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexImage2D(GL_TEXTURE_2D, 0, GL_INTENSITY, face->glyph->bitmap.width, face->glyph->bitmap.rows, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

				Character character = {
					tid,
					maths::vec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
					maths::vec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
					face->glyph->advance.x
				};
				m_CharCache.insert(std::pair<GLchar, Character>(c, character));

				glBindTexture(GL_TEXTURE_2D, 0);
			}
			FT_Done_Face(face);
			FT_Done_FreeType(ft);
		}
		void BatchRenderer2D::flush()
		{
			for (int i = 0; i < m_TextureSlots.size(); i++)
			{
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, m_TextureSlots[i]);
			}

			glBindVertexArray(m_VAO);
			m_IBO->Bind();

			glDrawElements(GL_TRIANGLES, m_IndexCount, GL_UNSIGNED_SHORT, NULL);

			glBindVertexArray(0);
			m_IBO->Unbind();

			m_IndexCount = 0;
		}
	}
}
