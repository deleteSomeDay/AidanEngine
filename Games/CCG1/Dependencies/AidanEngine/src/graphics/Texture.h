#pragma once
#include <GL/glew.h>
#include <string>
#include "../utils/ImageLoad.h"
namespace engine {
	namespace graphics {

		class Texture
		{
		private:
			std::string m_FileName;
			unsigned int m_RendererID;
			unsigned int tid;
			GLsizei m_Width, m_Height;

			//get rid of??
			int m_BPP;
		public:
			Texture();
			Texture(const std::string& filename);
			~Texture();

			void Bind() const;
			void Unbind() const;

			inline const unsigned int getWidth() const { return m_Width; }
			inline const unsigned int getHeight() const { return m_Height; }
			inline const unsigned int getID() const { return m_RendererID; }
		private:
			unsigned int load();

		};

	}
}
