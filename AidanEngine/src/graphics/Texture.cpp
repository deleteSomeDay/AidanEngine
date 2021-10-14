#include "Texture.h"
#include <iostream>
namespace engine {
	namespace graphics {

		Texture::Texture(const std::string& filename)
			: m_FileName(filename)
		{
			m_RendererID = load();
		}

		Texture::~Texture()
		{

		}

		unsigned int Texture::load()
		{
			unsigned char* pixels = load_image(m_FileName.c_str(), &m_Width, &m_Height, &m_BPP);

			unsigned int result; 
			glGenTextures(1, &result);
			glBindTexture(GL_TEXTURE_2D, result);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);	

			glBindTexture(GL_TEXTURE_2D, 0);
			
			//delete[] pixels

			return result;
		}

		void Texture::Bind() const
		{
			glBindTexture(GL_TEXTURE_2D, m_RendererID);
		}

		void Texture::Unbind() const
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}

	
	}
}