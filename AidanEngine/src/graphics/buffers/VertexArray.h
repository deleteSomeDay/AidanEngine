#pragma once
#include "Buffer.h"
#include <vector>


namespace engine {
	namespace graphics {

		class VertexArray
		{
		private:
			unsigned int m_RendererID;
			std::vector<Buffer*> m_Buffers;
		public:
			VertexArray();
			~VertexArray();

			void AddBuffer(Buffer* buffer, unsigned int index);

			void Bind() const;
			void Unbind() const;

		};
	}
}