#include "Simple2DRenderer.h"
#include "Renderable2D.h"
namespace engine {
	namespace graphics {

		void Simple2DRenderer::submit(const Renderable2D* renderable) 
		{
			m_RenderQueue.push_back((StaticSprite*)renderable);
		}
		void Simple2DRenderer::flush()
		{
			while (!m_RenderQueue.empty())
			{
				
				const StaticSprite* sprite = m_RenderQueue.front();
				
				
				sprite->getVAO()->Bind();
				sprite->getIBO()->Bind();

				sprite->getShader().SetUniformMat4f("ml_matrix", maths::mat4::translation(sprite->getPosition()));

				glDrawElements(GL_TRIANGLES, sprite->getIBO()->GetCount(), GL_UNSIGNED_SHORT, nullptr);

				sprite->getIBO()->Unbind();
				sprite->getVAO()->Unbind();

				m_RenderQueue.pop_front();
				
			}
		 }

	}
}
