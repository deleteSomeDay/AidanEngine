#pragma once
#include <cstddef>
#include "Renderer2D.h"
#include "Renderable2D.h"
#include "buffers\IndexBuffer.h"
#include <iostream>
#include <ft2build.h>
#include FT_FREETYPE_H

namespace engine {
	namespace graphics {

#define RENDERER_MAX_SPRITES 60000
#define RENDERER_VERTEX_SIZE sizeof(VertexData)
#define RENDERER_SPRITE_SIZE RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE  RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE RENDERER_MAX_SPRITES * 6

#define SHADER_VERTEX_INDEX 0
#define SHADER_UV_INDEX    1
#define SHADER_TID_INDEX   2
#define SHADER_COLOR_INDEX 3

		struct Character
		{
			unsigned int tid;
			maths::vec2 size;
			maths::vec2 bearing;
			unsigned int advance;
		};

		class BatchRenderer2D : public Renderer2D
		{
		private:
			GLuint m_VAO;
			IndexBuffer* m_IBO;
			GLsizei m_IndexCount;
			GLuint m_VBO;
			VertexData* m_Buffer;

			std::vector<unsigned int>  m_TextureSlots;

			std::unordered_map<GLchar, Character> m_CharCache;
		public:
			BatchRenderer2D();
			~BatchRenderer2D();
			void begin() override;
			void drawString(const std::string& string, const maths::vec3& position, const maths::vec4 color, float scale) override;
			void submit(const Renderable2D* renderable) override;
			 void end() override;
			 void flush() override;
			 void changeFont(const char* filename) override;
		private:
			void init();
		};
	}
}
