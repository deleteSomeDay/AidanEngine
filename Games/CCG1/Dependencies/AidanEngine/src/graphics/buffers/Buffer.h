#pragma once

#include <GL/glew.h>

namespace engine
{
	namespace graphics
	{
		class Buffer
		{
		private:
			unsigned int m_RendererID;
			unsigned int m_ComponentCount;
		public:
			//use short???
			Buffer(float* data, GLsizei size, unsigned int componentCount);
			~Buffer();
			void Bind() const;
			void Unbind() const;

			inline unsigned int getComponentCount() { return m_ComponentCount; }
		};
	}
}

