#include "Buffer.h"

namespace engine
{
	namespace graphics
	{
		Buffer::Buffer(float* data, GLsizei size, unsigned int componentCount) 
			: m_ComponentCount(componentCount)
		{


			glGenBuffers(1, &m_RendererID);
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
			glBufferData(GL_ARRAY_BUFFER, size * sizeof(float), data, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		};
		Buffer::~Buffer()
		{
			//glDeleteBuffers(1, &m_RendererID);
		}
		void Buffer::Bind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
		}
		void Buffer::Unbind() const
		{
			glBindBuffer(GL_ARRAY_BUFFER, 0);
		}
	}
}
