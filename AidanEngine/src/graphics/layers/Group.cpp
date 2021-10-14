#include "Group.h"

namespace engine {
	namespace graphics {

		Group::Group(const maths::mat4& matrix)
			: m_TransformationMatrix(matrix)
		{

		}

		void Group::add(Renderable2D* renderable)
		{
			m_Renderables.push_back(renderable);
			if (m_Renderables.size() == 1)
			{
				m_Size = (maths::vec2&)m_Renderables[0]->getSize();
			}
		}

		void Group::submit(Renderer2D* renderer) const
		{
			renderer->push(m_TransformationMatrix);

			for (const Renderable2D* renderable : m_Renderables)
			{
				renderable->submit(renderer);
			}

			renderer->pop();
		}
		void Group::setMatrix(maths::mat4 mat)
		{
			m_TransformationMatrix = mat;
			
		}
		maths::vec3 Group::getMatPos()
		{
		
			return maths::vec3(m_TransformationMatrix.elements[0 + 3 * 4],
				m_TransformationMatrix.elements[1 + 3 * 4],
				m_TransformationMatrix.elements[2 + 3 * 4]);
		}
		void Group::setSize(maths::vec2 size)
		{
			m_Size = size;
		}
		std::vector<Renderable2D*> Group::getRenderables()
		{
			return m_Renderables;
		}
	}
}
