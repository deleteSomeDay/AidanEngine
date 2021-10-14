#include "VertexArray.h"
#include "Buffer.h"
namespace engine
{
	namespace graphics
	{


		VertexArray::VertexArray()
		{
			glGenVertexArrays(1, &m_RendererID);
		}

		VertexArray::~VertexArray()
		{
			for (int i = 0; i < m_Buffers.size(); i++)
			{
				delete m_Buffers[i];
			}

			glDeleteVertexArrays(1, &m_RendererID);
		}
		void VertexArray::Bind() const
		{
			glBindVertexArray(m_RendererID);
		}

		void VertexArray::Unbind() const
		{
			glBindVertexArray(0);
		}

		void VertexArray::AddBuffer(Buffer* buffer, unsigned int index)
		{
			Bind();
			buffer->Bind();

			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, buffer->getComponentCount(), GL_FLOAT, GL_FALSE, 0, 0);

			buffer->Unbind();
			Unbind();

		}
	}
}