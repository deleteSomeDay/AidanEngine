#pragma once
#include "Renderable2D.h"
namespace engine {
	namespace graphics {


		class StaticSprite : public Renderable2D
		{
		private:
			IndexBuffer * m_IndexBuffer;
			VertexArray* m_VertexArray;
			Shader& m_Shader;
		public:
			StaticSprite(float x, float y, float width, float height, maths::vec4& color, Shader& shader);
			~StaticSprite();

			inline const IndexBuffer* getIBO() const { return m_IndexBuffer; }
			inline const VertexArray* getVAO() const { return m_VertexArray; }

			inline Shader& getShader() const { return m_Shader; }
		};


	}
}
