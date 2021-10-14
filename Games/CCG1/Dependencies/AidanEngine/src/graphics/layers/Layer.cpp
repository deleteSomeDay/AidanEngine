#include "Layer.h"

namespace engine {
	namespace graphics {

		Layer::Layer(Renderer2D* renderer, Shader* shader, maths::mat4 projection)
			: m_ProjectionMatrix(projection), m_Shader(shader), m_Renderer(renderer)
		{
			m_Shader->Bind();
			m_Shader->SetUniformMat4f("pr_matrix", m_ProjectionMatrix);
			m_Shader->Unbind();
		}
	
		

		Layer::~Layer()
		{
			delete m_Shader;
			delete m_Renderer;

			for (int i = 0; i < m_Renderables.size(); i++)
			{
				delete m_Renderables[i];
			}
		}

		void Layer::add(Renderable2D* renderable)
		{
			m_Renderables.push_back(renderable);
		}

		void Layer::render()
		{
		
			m_Shader->Bind();

			m_Renderer->begin();

			for (const Renderable2D* renderable : m_Renderables)
			{
				renderable->submit(m_Renderer);
			}

			m_Renderer->end();

			m_Renderer->flush();
		
		}
		void Layer::changeFont(const char* filename)
		{
			m_Renderer->changeFont(filename);
		}
	}
}
